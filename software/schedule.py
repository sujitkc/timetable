import random

heavy = (3, 2)
light = (2, 3)
vheavy = (3, 3)
vlight = (2, 2)

periodsPerDay = 8
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
	global periodsPerDay

	participants = range(50)
	slots = []
	for day in range(5):
		for period in range(periodsPerDay):
			slots.append((day, period)) 

	for x in range(20):
		subjects.append((x, loads[x % len(loads)])) # subject = (id, loadType)

	for p in participants:
		s = []
		for i in range(2):
			s1 = subjects[random.randint(0, len(subjects)) - 1]
			s.append(s1)
		participantSubjects[p] = set(s) # participantSubjects = {participant_1 : set[subject1, subject2, ...], participant_2 : set[subject1, subject2, ...],}

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
	return n1 in graph[n2] # n1 in graph[n2] <=> n2 in graph[n1] by construction

def schedule():
	global graph
	global participants
	global subjects
	global slots
	global subjectLoads
	global participantSubjects


# internal functions - begin

	theSchedule = {x : [] for x in slots}
	def isSlotAvailable(subject, groupSize, slot):
		day = slot[0]
		period = slot[1]
		if(period > (periodsPerDay - groupSize)):
			return False

		for x in range(groupSize):
			s2list = theSchedule[(day, period + x)]
			for s2 in s2list:
				if(areNeighbours(subject, s2)):
					return False
		return True

	def isSlotGroupAvailable(subject, slotGroup):
		groupSize = subject[1][1]
		for slot in slotGroup:
			if(not isSlotAvailable(subject, groupSize, slot)):
				return False
		return True


	def assignSubjectToSlot(subject, slot):
		groupSize = subject[1][1]
		for i in range(groupSize):
			theSchedule[(slot[0], slot[1] + i)].append(subject)

# internal functions - end
	goodChoices = {}
	goodChoices[heavy] = ((0, 2), (1, 3), (2, 4))
	goodChoices[light] = ((0, 2, 4), (1, 2, 3), (0, 1, 2), (2, 3, 4))
	goodChoices[vheavy] = ((0, 2, 4), (1, 2, 3), (0, 1, 2), (2, 3, 4))
	goodChoices[vlight] = ((0, 2), (1, 3), (2, 4))


	for subject in subjects:
		loadType = subject[1]
		choices = goodChoices[loadType]
		goodSlots = []
		for choice in choices:
			for period in range(periodsPerDay):
				l2 = []
				for day in choice:
					l2.append((day, period))
				goodSlots.append(l2)
		random.shuffle(goodSlots)
		for slotGroup in goodSlots:
			if(isSlotGroupAvailable(subject, slotGroup)):
				for slot in slotGroup:
					assignSubjectToSlot(subject, slot)
				break
				
	return theSchedule

def printSchedule(schedule):
	for day in range(5):
		for period in range(periodsPerDay):
			slot = (day, period)
			print str(schedule[slot]) + ' | ',
		print

setup()
makeGraph()
sched = schedule()
printSchedule(sched)
