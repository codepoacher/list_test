
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#include "list.h"

typedef int int32_t;

typedef struct role_attribute{
	char name[32];
	unsigned char age;
	unsigned char height;
	void (*func)(void *);
	void *args;
}S_ROLE_ATTRIBUTE;

typedef struct s_list_node{
	struct role_attribute role;
	struct list_head list;
}S_LIST_NODE;

LIST_HEAD(head_node);

void add_node(S_ROLE_ATTRIBUTE *role)
{
	S_LIST_NODE *node = (S_LIST_NODE *)malloc(sizeof(S_LIST_NODE));
	if(node == NULL) {
		return;
	}
	
	memcpy(&node->role, role, sizeof(S_ROLE_ATTRIBUTE));
	list_add_tail(&node->list, &head_node);
}

void del_node(char *name)
{
	struct list_head *pos, *n;
	S_LIST_NODE *node;

	list_for_each_safe(pos, n, &head_node){
		node = list_entry(pos, S_LIST_NODE, list);
		if(strcmp(node->role.name, name) == 0) {
			list_del(pos);
			free(node);
		}		
	}
}

void func_node(char *name)
{
	struct list_head *pos;
	S_LIST_NODE *node;

	list_for_each(pos, &head_node){
		node = list_entry(pos, S_LIST_NODE,list);
		if(node->role.func != NULL) {
			if(name == NULL){
				node->role.func(node->role.args);
			} else {
				if(strcmp(node->role.name, name) == 0) {
					node->role.func(node->role.args);
				}
			}
		}
	}
}

void print_node()
{
	struct list_head *pos;
	S_LIST_NODE *node;
	list_for_each(pos, &head_node){
		node = list_entry(pos, S_LIST_NODE, list);
		printf("#############\r\n");
		printf("name:%s\r\n",node->role.name);
		printf("age:%d\r\n",node->role.age);
		printf("height:%d\r\n",node->role.height);
	}
	printf("#############\r\n");
}

void abcd_func(void *args)
{
	printf("%s,line=%d\r\n",__FUNCTION__, __LINE__);
}

void abce_func(void *args)
{
	printf("%s,line=%d,%s\r\n",__FUNCTION__, __LINE__,(char *)args);
}

int main()
{
	struct list_head *pos;
	S_LIST_NODE *node; 
	S_ROLE_ATTRIBUTE role = {0};
	
	memcpy(role.name, "abcd", strlen("abcd"));
	role.age = 10;
	role.height = 175;
	role.func = abcd_func;
	role.args = NULL;
	add_node(&role);

	memset(&role, 0, sizeof(S_ROLE_ATTRIBUTE));
	memcpy(role.name, "abce", strlen("abce"));
	role.age = 11;
	role.height = 171;
	role.func = abce_func;
	role.args = "1234";
	add_node(&role);

	func_node(NULL);
	func_node("abce");
	print_node();
	
	del_node("abce");
	func_node(NULL);
	print_node();
	
	return 0;
}

