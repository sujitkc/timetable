#!/usr/bin/python

import sys
import os
import random

from string import *

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
			numberOfCourses = len(fields)
			for i in range(len(fields)):
				if(strip(fields[i]) == 'Yes'):
					if(numberOfParticipants not in resourceMap):
						resourceMap[numberOfParticipants] = []
					resourceMap[numberOfParticipants].append(i)
		numberOfParticipants = numberOfParticipants + 1
	fin.close()
	return resourceMap

def invert(resourceMap):
	dic = {}
	for key in resourceMap:
		for value in resourceMap[key]:
			if value not in dic:
				dic[value] = []
			dic[value].append(key)
	return dic

def printPretty(resourceMap):
	for key in resourceMap:
		for value in resourceMap[key]:
			print 'p' + str(key) + ' | c' + str(value) + ' | i' + str(value)

def convert():
	if(len(sys.argv) != 2):
		print 'usage: gen-report.py dirname'
		sys.exit()

	dirName = sys.argv[1]
	if(not os.path.exists(dirName)):
		print dirName + ": directory does not exist."
		sys.exit(1)
	resourceMap = readResourceMap(dirName + '/resourcemap.csv')
#	print 'resourceMap = ', resourceMap
	printPretty(resourceMap)
#	dic = invert(resourceMap)
#	print 'dic = ', dic

convert()

