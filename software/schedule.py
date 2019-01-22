#!/usr/bin/python

import sys
import os
import random

from string import *



heavy = (3, 2)
light = (2, 3)
vheavy = (3, 3)
vlight = (2, 2)

periodsPerDay = 8
loads = (heavy, light, vheavy, vlight)

dirName = ''
participants = []
courses = []
slots = []

courseLoads = {}
resourceMap = {}

def newCourse(id, load):
	return (id, load)

def getCourseId(s):
	return s[0]

def getCourseLoadType(s):
	return s[1]

def getLoadDuration(l):
	return l[0]

def getLoadGroupSize(l):
	return l[1]

def newSlot(day, period):
	return (day, period)

def getSlotDay(s):
	return s[0]

def getSlotPeriod(s):
	return s[1]

def readCourses(fileName):
	if(not os.path.isfile(fileName)):
		print fileName + ": file does not exist."
		sys.exit(1)

	fin = open(fileName, 'r')
	courseId = 0
	courses = []
	while(True):
		line = fin.readline()
		if(line == ''):
			break
		else:
			sline = strip(line)
			if(sline == 'vlight'):
				courses.append(newCourse(courseId, vlight))
			elif(sline == 'light'):	
				courses.append(newCourse(courseId, light))
			elif(sline == 'heavy'):		
				courses.append(newCourse(courseId, heavy))
			elif(sline == 'vheavy'):		
				courses.append(newCourse(courseId, vheavy))
		courseId = courseId + 1
	fin.close()
	return courses
	
def readResourceMap(fileName, participants):
	if(not os.path.isfile(fileName)):
		print fileName + ": file does not exist."
		sys.exit(1)

	fin = open(fileName, 'r')
	resourceMap = {}

	numberOfParticipants = 0
	while(True):
		line = fin.readline()
		if(line == ''):
			break
		else:
			fields = str.split(line, "|")
			numberOfCourses = len(fields)
			for i in range(len(fields)):
				if(strip(fields[i]) == 'Yes'):
					if(numberOfParticipants not in resourceMap):
						resourceMap[numberOfParticipants] = []
					resourceMap[numberOfParticipants].append(courses[i])
		numberOfParticipants = numberOfParticipants + 1
	fin.close()
	return resourceMap

def setup():
	global courses
	global slots
	global resourceMap
	global periodsPerDay
	global dirName
	if(len(sys.argv) != 2):
		print 'usage: gen-report.py dirname'
		sys.exit()

	dirName = sys.argv[1]
	if(not os.path.exists(dirName)):
		print dirName + ": directory does not exist."
		sys.exit(1)


	slots = [newSlot(day, period) for day in range(5) for period in range(periodsPerDay)]
	courses = readCourses(dirName + '/courses.csv')
	resourceMap = readResourceMap(dirName + '/resourcemap.csv', participants)

def makeGraph():
	graph = {}
	def haveCommonParticipant(s1, s2):
		for p in range(len(participants)):
			if(s1 in resourceMap[p] and s2 in resourceMap[p]):
				return True
		return False

	for s in courses:
		graph[s] = []
	for s1 in courses:
		for s2 in courses:
			if(s2 not in graph[s1]):
				if(haveCommonParticipant(s1, s2)):
					graph[s1].append(s2)
					graph[s2].append(s1)
	return graph

def areNeighbours(n1, n2, g):
	return n1 in g[n2] # n1 in graph[n2] <=> n2 in graph[n1] by construction

def getCoursesForSlot(slot, schedule):
	return [c for c in schedule if slot in schedule[c]]

def schedule(graph):
	global participants
	global courses
	global slots
	global courseLoads
	global resourceMap

	goodChoices = {}
	goodChoices[heavy] = ((0, 2), (1, 3), (2, 4))
	goodChoices[light] = ((0, 2, 4), (1, 2, 3), (0, 1, 2), (2, 3, 4))
	goodChoices[vheavy] = ((0, 2, 4), (1, 2, 3), (0, 1, 2), (2, 3, 4))
	goodChoices[vlight] = ((0, 2), (1, 3), (2, 4))

	empty = {}
	for course in courses:
		empty[course] = []

	def satisfy(index, sched):
		course = courses[index]

		def isSlotAvailable(slot):
			day = getSlotDay(slot)
			period = getSlotPeriod(slot)
			groupSize = getLoadGroupSize(getCourseLoadType(course))
			if(period > (periodsPerDay - groupSize)):
				return False

			for x in range(groupSize):
				courselist = getCoursesForSlot(newSlot(day, period + x), sched)
				for c in courselist:
					if(areNeighbours(course, c, graph)):
						return False
			return True

		def isSlotGroupAvailable(slotGroup):
			groupSize = getLoadGroupSize(getCourseLoadType(course))
			for slot in slotGroup:
				if(not isSlotAvailable(slot)):
					return False
			return True
		
		def getPermittedSlotGroups():
			loadType = getCourseLoadType(course)
			choices = goodChoices[loadType]
			goodSlotGroups = []
			for choice in choices:
				for period in range(periodsPerDay):
					l2 = []
					for day in choice:
						l2.append(newSlot(day, period))
					if(isSlotGroupAvailable(l2)):
						goodSlotGroups.append(l2)
			random.shuffle(goodSlotGroups)
			return goodSlotGroups

		new = {}
		for c in sched:
			new[c] = sched[c]
		permitted = getPermittedSlotGroups()
		if(permitted == []):
			return empty
		if(index == len(courses) - 1):
			new[course] = [newSlot(getSlotDay(p), getSlotPeriod(p) + i) for p in permitted[0] for i in range(getLoadDuration(getCourseLoadType(course)))]
			return new
		for slotGroup in permitted:
			new[course] = [newSlot(getSlotDay(p), getSlotPeriod(p) + i) for p in slotGroup for i in range(getLoadDuration(getCourseLoadType(course)))]
			result = satisfy(index + 1, new)
			if(result != empty):
				return result
		return empty
				
	theSchedule = {c : [] for c in courses}
	return satisfy(0, theSchedule)

def printResourceMap(matrix):
	for p in participants:
		for s in courses:
			if(s in matrix[p]):
				print 'Yes |',
			else:
				print 'No  |',
		print

def printSchedule(schedule):
	for day in range(5):
		periodNumber = 0
		for period in range(periodsPerDay):
			slot = newSlot(day, period)
			clist = getCoursesForSlot(slot, schedule)
			if(clist == []):
				print ' ',
			else:
				courseNumber = 0
				for course in clist:
					print str(getCourseId(course)),
					if(courseNumber != len(clist) - 1):
						print ', ',
					courseNumber = courseNumber + 1
			if(periodNumber != periodsPerDay - 1):
				print '|',
		print
		periodNumber = periodNumber + 1
	

def printScheduleToFile(schedule, fileName):
	fout = open(fileName, 'w')
	for day in range(5):
		periodNumber = 0
		for period in range(periodsPerDay):
			slot = newSlot(day, period)
			clist = getCoursesForSlot(slot, schedule)
			if(courses == []):
				print ' '
			else:
				courseNumber = 0
				for course in clist:
					fout.write(str(getCourseId(course)))
					if(courseNumber != len(courses) - 1):
						fout.write(', ')
					courseNumber = courseNumber + 1
			if(periodNumber != periodsPerDay - 1):
				fout.write(' | ')
		fout.write('\n')
		periodNumber = periodNumber + 1
	fout.close()

setup()
g = makeGraph()
sched = schedule(g)
printSchedule(sched)
printScheduleToFile(sched, dirName + '/sch.csv')
