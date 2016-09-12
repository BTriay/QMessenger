#ifndef PARSERCONST_H
#define PARSERCONST_H

#define ROOM_MSG				1
    #define NEW_MSG                 1
    #define ROOM_USERS              2
    #define NEW_JOINER              3
    #define LEAVER                  4
#define KEEPALIVE				2
#define PRESENCE_UPDATE			3
    #define ONLINE                  1
    #define AWAY                    2
    #define DO_NOT_DISTURB          3
    #define OFFLINE                 4
    #define	INVISIBLE               5
#define NEW_ROOM				4
#define JOIN_THE_ROOM			5
#define LEAVE_THE_ROOM			6
#define BE_MY_FRIEND			7
    #define NEW_REQUEST             1
    #define UNKNOWN_USER            2
#define I_AM_YOUR_FRIEND		8
#define WE_ARE_NOT_FRIENDS      9
#define FRIENDS_LIST            10
#define HELLO					11
    #define WRONG_PWD				1
    #define DONT_KNOW_YOU           2
    #define WELCOME_BACK            3
		#define ALREADY_LOGGED					4
#define NEW_USER				12
    #define USERNAME_OK             1
    #define USERNAME_TAKEN          2
#define DELETE_ME				13
    #define WRONG_PWD				1
    #define DELETION_OK				2
#define VERSION_NO				14
#define INVALID_MSG				15

#endif
