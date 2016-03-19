/*
 * friendrecommender.cpp
 *
 *  Created on: 19 Mar 2016
 *      Author: zeroxwj
 */

#include <cstdio>
#include <cstring>
#include <cassert>
#include <string>
#include <set>
#include <vector>
#include <map>


// Global variables
unsigned int threshold = 0;	// m value
unsigned long userNum = 0;	// number of users

std::map<std::string, unsigned long> username_dic;

typedef std::vector<std::set<unsigned long> > HashedAdjacentList;
HashedAdjacentList hashedList;

//typedef std::vector<std::vector<unsigned long> > AdjacentList;
//AdjacentList friendList;


// new added functions
/*********************************************************

/*********************************************************

/**
 * Initialize social network. Clear all global variables
 * m: the threshold value
 */
void Init(unsigned int m) {
	// Clear global variables
	username_dic.clear();
	userNum = 0;

	for (int i = 0; i < userNum; ++i) {
		hashedList[i].clear();
	}
	hashedList.clear();

	threshold = m;

}

/**
 * pUser: not null
 *
 * return 0 successful
 * 		  -1 error
 */
int CreateUser(const char* pUser) {

	if ((NULL == pUser)) {
		return -1;
	} else if ((strlen(pUser) < 1) || (strlen(pUser) > 20)) {	//not in range [1...20]
		return -1;
	} else if (username_dic.find(std::string(pUser)) != username_dic.end()){	// if user already exists
		return -1;
	}

	username_dic[std::string(pUser)] = userNum;

	// Create adjacent lists
	hashedList.push_back(std::set<unsigned long>());

	userNum++;

	return 0;
}


/**
 * return 0 success
 *        -1 error
 */
int AddFriends(const char* pUser1, const char* pUser2) {

	unsigned long user1Id = 0;
	unsigned long user2Id = 0;

	if ((NULL == pUser1) || (NULL == pUser2)) {
		return -1;
	} else if ((username_dic.find(std::string(pUser1)) == username_dic.end()) ||
			(username_dic.find(std::string(pUser2)) == username_dic.end())) {
		// User does not exist
		return -1;
	} else if (strcmp(pUser1, pUser2) == 0)	{	// self-friending
		return -1;
	} else {
		user1Id = username_dic[std::string(pUser1)];
		user2Id = username_dic[std::string(pUser2)];

		// already as friends. XU: really necessary?
		if (hashedList[user1Id].find(user2Id) != hashedList[user1Id].end()) {
			return -1;
		}
	}

	// Add as mutual friends
	friendList[user1Id].push_back(user2Id);
	friendList[user2Id].push_back(user1Id);

	hashedList[user1Id][user2Id] = true;
	hashedList[user2Id][user1Id] = true;


	return 0;
}

/**
 * After no more friendship can be created
 */
int GetUserFriendNum(const char* pUser, unsigned int* pFriendNum) {

	if (NULL == pUser) {
		return -1;
	} else if (username_dic.find(std::string(pUser)) == username_dic.end())	{	//user does not exist
		return -1;
	}

	// Get number of direct friends
	unsigned long userId = username_dic[std::string(pUser)];
	*pFriendNum = hashedList[userId].size();

	return 0;
}


/**
 * After no more friendship can be created
 */
int IsFriend(const char* pUser1, const char* pUser2) {

	if ((NULL == pUser1) || (NULL == pUser2)) {
		return -1;
	} else if ((username_dic.find(std::string(pUser1)) == username_dic.end()) ||
			(username_dic.find(std::string(pUser2)) == username_dic.end()))	{	//user does not exist
		return -1;
	}

	unsigned long user1Id = username_dic[std::string(pUser1)];
	unsigned long user2Id = username_dic[std::string(pUser2)];

	if (hashedList[user1Id].find(user2Id) != hashedList[user1Id].end()) {
		return 0;
	} else {
		return -1;
	}
}


int main() {

	Init(2);

	CreateUser("Jack");
	CreateUser("Peter");
	CreateUser("Bruce");
	CreateUser("Smith");
	CreateUser("Tom");

	AddFriends("Jack", "Peter");
	AddFriends("Jack", "Tom");
	AddFriends("Smith", "Peter");

	unsigned int num = 0;
	GetUserFriendNum("Jack", &num);
	assert(num == 5);

	GetUserFriendNum("Peter", &num);
	assert(num == 4);

	assert(0 == IsFriend("Jack", "Rose"));
	assert(0 == IsFriend("Jack", "Frank"));
	assert(0 == IsFriend("Rose", "Bruce"));

	return 0;
}



