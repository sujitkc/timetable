#!/usr/bin/python

import sys
import os
import random

from string import *

periodsPerDay = 4
daysPerWeek = 6

goodSlots = [(x, y) for x in range(daysPerWeek) for y in range(daysPerWeek) if x != y and x < y]

dirName = ''
participants = []
courses = []
slots = []

resourceMap = {}

def newCourse(id):
	return (id)

def getCourseId(s):
	return s[0]

def newSlot(day, period):
	return (day, period)

def getSlotDay(s):
	return s[0]

def getSlotPeriod(s):
	return s[1]

def readResourceMap(fileName):
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
			p1 = strip(fields[0])
			course = strip(fields[1])
			p2 = strip(fields[2])
			if course not in courses:
				courses.append(course)
			if course not in resourceMap:
				resourceMap[course] = []
			if p1 not in resourceMap[course]:
				resourceMap[course].append(p1)
			if p2 not in resourceMap[course]:
				resourceMap[course].append(p2)
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


	slots = [newSlot(day, period) for day in range(daysPerWeek) for period in range(periodsPerDay)]
	resourceMap = readResourceMap(dirName + '/resourcemap.csv')

def makeGraph(resourceMap):
	graph = {}
	def haveCommonParticipant(c1, c2):
		for p in resourceMap[c1]:
			if p in resourceMap[c2]:
				return True
		return False

	for c in courses:
		graph[c] = []
	for c1 in courses:
		for c2 in courses:
			if(c2 not in graph[c1]):
				if(haveCommonParticipant(c1, c2)):
					graph[c1].append(c2)
					graph[c2].append(c1)
	return graph

def areNeighbours(n1, n2, g):
	return n1 in g[n2] # n1 in graph[n2] <=> n2 in graph[n1] by construction

def getCoursesForSlot(slot, schedule):
	return [c for c in schedule if slot in schedule[c]]

def schedule(graph):
	global slots
	global resourceMap

	empty = {}
	for course in courses:
		empty[course] = []

	def satisfy(index, sched):
		course = courses[index]

		def isSlotAvailable(slot):
			day = getSlotDay(slot)
			period = getSlotPeriod(slot)
			groupSize = 1
			if(period > (periodsPerDay - groupSize)):
				return False

			for x in range(groupSize):
				courselist = getCoursesForSlot(newSlot(day, period + x), sched)
				for c in courselist:
					if(areNeighbours(course, c, graph)):
						return False
			return True

		def isSlotGroupAvailable(slotGroup):
			for slot in slotGroup:
				if(not isSlotAvailable(slot)):
					return False
			return True
		
		def getPermittedSlotGroups():
			choices = goodSlots
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
			new[course] = [newSlot(getSlotDay(p), getSlotPeriod(p) + i) for p in permitted[0] for i in range(1)]
			return new
		for slotGroup in permitted:
			new[course] = [newSlot(getSlotDay(p), getSlotPeriod(p) + i) for p in slotGroup for i in range(1)]
			result = satisfy(index + 1, new)
			if(result != empty):
				return result
		return empty
				
	theSchedule = {c : [] for c in courses}
	theSchedule = satisfy(0, theSchedule)
	return theSchedule

def printResourceMap(matrix):
	for p in participants:
		for s in courses:
			if(s in matrix[p]):
				print 'Yes |',
			else:
				print 'No  |',
		print

def printSchedule(schedule):
	for day in range(daysPerWeek):
		periodNumber = 0
		for period in range(periodsPerDay):
			slot = newSlot(day, period)
			clist = getCoursesForSlot(slot, schedule)
			if(clist == []):
				print ' ',
			else:
				courseNumber = 0
				for course in clist:
					print course,
					if(courseNumber != len(clist) - 1):
						print ', ',
					courseNumber = courseNumber + 1
			if(periodNumber != periodsPerDay - 1):
				print '|',
		print
		periodNumber = periodNumber + 1
	

def printScheduleToFile(schedule, fileName):
	fout = open(fileName, 'w')
	for day in range(daysPerWeek):
		periodNumber = 0
		for period in range(periodsPerDay):
			slot = newSlot(day, period)
			clist = getCoursesForSlot(slot, schedule)
			if(courses == []):
				print ' '
			else:
				courseNumber = 0
				for course in clist:
					fout.write(course)
					if(courseNumber != len(courses) - 1):
						fout.write(', ')
					courseNumber = courseNumber + 1
			if(periodNumber != periodsPerDay - 1):
				fout.write(' | ')
		fout.write('\n')
		periodNumber = periodNumber + 1
	fout.close()

setup()
print str(len(resourceMap)) + ' courses:'
print dict.keys(resourceMap)
g = makeGraph(resourceMap)
sched = schedule(g)
if(sched == empty):
	print "Couldn't find solution."
else:
	print sched
printSchedule(sched)
printScheduleToFile(sched, dirName + '/sch.csv')
