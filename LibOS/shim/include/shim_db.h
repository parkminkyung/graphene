
#ifndef _SHIM_DB_H_
#define _SHIM_DB_H_

#include <stdbool.h>

#include <shim_types.h>
#include <shim_defs.h>
#include <shim_handle.h>
#include <shim_utils.h>

#include <pal.h>
#include <list.h>


enum userdata_age {
	AGE_10,
	AGE_20,
	AGE_30,
	AGE_40,
	AGE_50
};

enum userdata_gender {
	WOMAN,
	MAN
};

enum userdata_interest1 {
	INTER1_1,
	INTER1_2,
	INTER1_3,
	INTER1_4,
	INTER1_5,
	INTER1_6
};

enum userdata_interest2 {
	INTER2_1,
	INTER2_2,
	INTER2_3,
	INTER2_4,
	INTER2_5,
	INTER2_6
};

enum userdata_interest3 {
	INTER3_1,
	INTER3_2,
	INTER3_3,
	INTER3_4,
	INTER3_5,
	INTER3_6
};

typedef struct userdata_entry {
	enum userdata_age age; 
	enum userdata_gender gender;
	enum userdata_interest1 interest1;
	enum userdata_interest2 interest2;
	enum userdata_interest3 interest3;
} userdata_entry;


typedef struct campaign_entry {
	int id; // neccessary?
	char *meta;
	int budget;
	char *file; // file pointer
	struct userdata_entry *_userdata_table;
	int _userdata_alloc;
	int _userdata_index;
} campaign_entry;


int init_db (void);
int parsing_userdata(char *raw_data, userdata_entry *entry);
int parsing_campaign(char *raw_data, campaign_entry *entry);
int insert_campaign(char *raw_data);
int update_campaign(int budget, int id);
int insert_userdata(char* raw_data, campaign_entry *entry);

#endif
