import random

heavy = (3, 2)
light = (2, 3)
vheavy = (3, 3)
vlight = (2, 2)

loads = (heavy, light, vheavy, vlight)

participants = []
subjects = []
slots = []

subjectLoads = {}
participantSubjects = {}

def setup():
	global participants
	global subjects
	global slots
	global subjectLoads
	global participantSubjects

	participants = [x + 1 for x in range(50)]
	subjects = [x + 1 for x in range(20)]
	slots = [x + 1 for x in range(60)]

	for x in range(len(subjects)):
		subjectLoads[participants[x]] = loads[x % len(loads)]

	for p in participants:
		s = []
		for i in range(2):
			s1 = subjects[random.randint(0, len(subjects)) - 1]
			s.append(s1)
		participantSubjects[p] = set(s)

graph = {}
def makeGraph():
	global graph
	def haveCommonParticipant(s1, s2):
		for p in participants:
			if(s1 in participantSubjects[p] and s2 in participantSubjects[p]):
				return True
		return False


	for s in subjects:
		graph[s] = []
	for s1 in subjects:
		for s2 in subjects:
			if(s2 not in graph[s1]):
				if(haveCommonParticipant(s1, s2)):
					graph[s1].append(s2)
					graph[s2].append(s1)

def areNeighbours(n1, n2):
	global graph
	return n1 in graph[n2]

def schedule():
	global graph
	global participants
	global subjects
	global slots
	global subjectLoads
	global participantSubjects

	goodChoices = {}
	goodChoices[heavy] = ((0, 2), (1, 3), (3, 5))
	goodChoices[light] = ((1, 3, 5), (1, 2, 3), (2, 3, 4), (3, 4, 5))
	goodChoices[vheavy] = ((1, 3, 5), (1, 2, 3), (2, 3, 4), (3, 4, 5))
	goodChoices[vlight] = ((0, 2), (1, 3), (3, 5))

	theSchedule = {x : [] for x in slots}
	def isSlotAvailable(subject, groupSize, slot):
		if(slot > (len(slots) - groupSize)):
			return False

		for x in range(groupSize):
			s2list = theSchedule[slot + x]
			for s2 in s2list:
				if(areNeighbours(subject, s2)):
					return False
		return True



	flat = []
	for s in subjectLoads:
		load = subjectLoads[s]
		groupSize = load[0]
		lectures = load[1]
		entry = [(s, groupSize)] * lectures
		flat.extend(entry)	

#	print 'flat = ' + str(flat)
	for entry in flat:
		s = entry[0]
		groupSize = entry[1]
		for slot in slots:
			done = False
			if(isSlotAvailable(s, groupSize, slot)):
				for x in range(groupSize):
					theSchedule[slot + x].append(s)
				done = True
			if(done == True):
				break
	return theSchedule

setup()
makeGraph()
print schedule()
