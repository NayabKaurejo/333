//p3.c Greg O'Connor (GO198787) & Nayab Kaurejo (NK794194)
//Group members from Friday 1:40PM lab
//file contains main by Greg O'Connor & Nayab Kaurejo
//Insert, Delete, Forced Delete, Print Suffix by Greg O'Connor
//Print List, Print Statistics, Print Count Range, Print Prefix by Nayab Kaurejo
//And a singly linked list struct by Greg O'Connor & Nayab Kaurejo.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXSYMBOL 11 //maximum symbol size for each node including \0
#define MAXIN 4 //maximum input characters including \0

//Singly linked list
typedef struct LinkedList
{
	char symbol [MAXSYMBOL]; //each node can have a symbol 10 characters long (11 including \0)
	int count; //each node has a counter for how many times its symbol has been added to the list.
	struct LinkedList *next; //pointer to the next node
} NODE, *PTRNODE; //for shorthand usage

//function prototypes:
void insert(char *, PTRNODE*); //Greg O'Connor
void delete(char *, PTRNODE*); //Greg O'Connor
void fDelete(int, PTRNODE*); //Greg O'Connor
void pStat(PTRNODE); //Nayab Kaurejo
void pList(const PTRNODE); //Nayab Kaurejo
void pRange(int, int, const PTRNODE); //Nayab Kaurejo
void pPrefix(char *, const PTRNODE); //Nayab Kaurejo
void pSuffix(char *, const PTRNODE); //Greg O'Connor

//Main function which t2akes user input and acts on a linked list based on that input.
//assumes correct input.
int main(void)
{
	PTRNODE list; //the list that holds the test data.
	char cmd [MAXIN]; //string to hold the command from the console.
	char inStr [MAXSYMBOL]; //string to hold any input from the console other than the command.
	int inInt1, inInt2; //integers to hold any input from the console that isn't a string.
	inInt1 = inInt2 = 0; //initializing input integers to zero.
	
	
	//get input from user (scanf)
	scanf("%s", cmd);

	list = NULL;
	//start loop
	while(1)
	{

		//process input from user (switch)
	
		if(strcmp(cmd, "end")==0) break;//--------------------------------------------------end the program.
		if(strcmp(cmd, "ins")==0){scanf("%s", inStr); insert(inStr, &list);}//--------------enters the insert function with inStr and a pointer to a pointer to a list.
		if(strcmp(cmd, "del")==0){scanf("%s", inStr); delete(inStr, &list);}//--------------enters the delete function with inStr and a pointer to a pointer to a list.
		if(strcmp(cmd, "fde")==0){scanf("%d", &inInt1); fDelete(inInt1, &list);}//----------enters the forced delete function with inInt1 and a pointer to a pointer to a list.
		if(strcmp(cmd, "pst")==0){pStat(list);}//-------------------------------------------enters the statistics function with a list pointer.
		if(strcmp(cmd, "prl")==0){pList(list);}//-------------------------------------------enters the print list function with a list pointer.
		if(strcmp(cmd, "pcr")==0){scanf("%d %d", &inInt1, &inInt2); pRange(inInt1, inInt2, list);}//enters the print range function with inInt1, inInt2, and a list pointer.
		if(strcmp(cmd, "ppr")==0){scanf("%s", inStr); pPrefix(inStr, list);}//--------------enters the print prefix function with inStr and a list pointer.
		if(strcmp(cmd, "psu")==0){scanf("%s", inStr); pSuffix(inStr, list);}//--------------enters the print suffix function with inStr and a list pointer.
		scanf("%s", cmd); //get the next command from the console.
	}//end loop
	return 0;
}//end main

//Insert function - Greg O'Connor
//Inserts a node into the given list.
//If the node exists, simply increments its count.
//If the node is then out of place, removes the node and places it in the appropriate place.
void insert(char * s, PTRNODE * list)
{
	PTRNODE head = *list; //to simplify referencing the list
	PTRNODE current = head; //keep track of the current node being traversed
	PTRNODE previous = current; //keep track of the last node traversed

	if(head == NULL)//case 1: the list is empty
	{
		if((head = malloc(sizeof(NODE))) == NULL)//dynamically create a new node
		{
			printf("An error occured while creating a new node (insert function). Terminating program.\n");
			exit(1);
		}
		head->next = NULL;		//set its next pointer to null
		strcpy(head->symbol, s);	//set its symbol to the given string
		head->count = 1;		//set its count to one
		*list = head;			//fix the list to point to the head.
		return;				//leave the insert function
	}//end if
	while(current != NULL)//case 2: the list is not empty
	{
		if(!strcmp(current->symbol, s))//if the current symbol matches what's being added to the list
		{
			current->count++;
			if(previous->count < current->count) //the list is out of order now.
			{
				previous->next = current->next;//remove the current node from the list
				previous = head; //reset previous pointer;
				while(previous != NULL)
				{
					if(previous->count >= current->count && previous->next != NULL && previous->next->count <= current->count)
					{//found a suitable place for the node
						current->next = previous->next;
						previous->next = current; //insert the node in its proper place.
						
						return;
					}
					previous = previous->next;//go the the next node...
				}
				//got here without returning anything... must need to insert at the head.
				current->next = head;//append the list to current;
				*list = current; //point the head of the list to current;
			}
			return;
		}
		previous = current; //step to the next node.
		current = current->next;
	}
	//if we got this far, we didn't find the symbol in the list
	//since we got here, it just so happens that previous is the last node in the list;
	if(previous != NULL && current == NULL)
	{
		if((previous->next = malloc(sizeof(NODE)))== NULL)//dynamically create new node, check if it's NULL
		{
			printf("An error occured while creating a new node (insert function). Terminating program.\n");
			exit(1);//exit with error value 1.
		}
		current = previous->next; //current now points to the last node in the list.
		current->next = NULL; //make sure current's next is NULL.
		strcpy(current->symbol,s); //put the new symbol into the current node.
		current->count = 1; //fix the count of the current node.
	}
	head = *list;//fix the list so that *list points to head.
	return; //done
}//end insert

//Delete function - Greg O'Connor
//Decrements count of the node matching the character * s
//If that node's count is now zero, removes that node
//If decrementing the count results in the node being out of place, removes the node and places it in the correct location.
void delete(char * s, PTRNODE * list)
{
	PTRNODE head = *list; //to simplify referencing the list
	PTRNODE current = head; //keep track of the current node being traversed
	PTRNODE previous = current; //keep track of the last node traversed

	if(head != NULL)//so long as the list isn't empty
	{
		while(current != NULL)//loop through the list until we find a NULL node
		{
			if(!strcmp(current->symbol, s))//found the symbol being deleted
			{
				current->count--;
				if(current->count == 0)//if that was all there was left in this node...
				{
					if(current == head)
					{
						*list = head->next; //point the list to the next thing in the list (even if it's NULL).
					}
					previous->next = current->next; //delete it.
					free(current); //dynamically free up the node at current.
					return;//done.
				}
				else
				if(current->next != NULL && current->next->count > current->count)//if the list is now out of order...
				{
					previous->next = current->next; //remove the offending node
					previous = head; //start at the beginning
					while(previous != NULL) 
					{
						if(previous->count >= current->count && previous->next != NULL && previous->next->count <= current->count) //found a suitable place for the node
						{
							current->next = previous->next;//insert current after previous but before previous->next.
							previous->next = current; //step through the list.
							return;
						}
						previous = previous->next;// use previous as if it were the "current" counter, because current is holding the node that was out of order.
					}
					//we got here, so previous is NULL.
					previous = current; //insert current at the end of the list
					current->next = NULL; //make current's next pointer NULL.
					return; //done.
				}
			}
			previous = current; //step to the next node
			current = current->next;
		}
	}
	return;//done.
}//end delete function

//Forced Delete function - Greg O'Connor
//Removes all nodes in the list that have a count less than or equal to x
void fDelete(int x, PTRNODE * list)
{
	PTRNODE head = *list; //to make dereferenceing the list easier.
	PTRNODE current; //don't bother initializing this yet.
	PTRNODE previous = head; //initialize this to the head of the list.
	if(head != NULL)
	{
		current = head->next; //skip the head for now, we will deal with the head node at the end.
	}
	
	while(current != NULL) //step through the list
	{
		if(current->count <= x)
		{
			previous->next = current->next; //remove current from the list
			free(current); //free up its memory.
			current = NULL; //no, really, why aren't you gone yet?
			current = previous->next; //now, resume current position.
		}
		else
		{
			previous = current;//just step through the list normally.
			current = current->next;
		}
	}
	//check to see if the head is less than x
	if(head != NULL && head->count <= x)
	{	//if we get here, the rest of the list should already be gone.
		free(*list);//deleted the whole list.
		*list = NULL; //no, really, delete the whole list.
	}
}//end forced delete.

//Print statistics function - Nayab Kaurejo
//prints the number of nodes, maximum count, minimum count, and average of counts in the supplied list
void pStat(const PTRNODE list) // this function will print statistics of a list 
{
	PTRNODE head = list;
	int count , max , min , sum; // variables for the number of nodes, max and min value
	float avg;
	count = max = min = sum = 0; // given the variables initialization to 0
	avg = 0; // a float to get the real number as an average, and also a sum is declared to add all the nodes
	printf("\n"); fflush(stdout);
	if (head == NULL) // if head is null the program will print empty and stop
	{
		printf("The list is empty."); fflush(stdout);
	}
	else
	{
		min = head->count; // points min to the first node of the list
	}

	while (head != NULL)//step through the list until the NULL pointer at the end.
	{
		count++; // iterates the count for every node in the list 
		head = head ->next; // goes through each node of list
	}
	head = list; // after the while loop, head gets pointed back the front of the list
	while (head != NULL)//step through the list until the NULL pointer at the end.
	{
		if (head->count > max){ // if there is a larger value in the list
			max = head->count; // it will be assigned to the max variable
		}
		head = head->next;  // goes through each node of list
	}
	head = list; // after the while loop, head gets pointed back the front of the list
	while (head != NULL) //step through the list until the NULL pointer at the end.
	{
		if (head->count < min){ // if there is a lesser value in the list
			min = head->count; // it will be assigned to the min variable
		}
		head = head->next; // iterates through each node of the list
	}
	head = list; // after the while loop, head gets pointed back the front of the list
	while (head != NULL)//step through the list until the NULL pointer at the end.
	{
		sum += head->count; // adding all the counts of each node into the sum variable
		head = head->next; // iterates through each node of the list
	}
	avg = (float)sum/(float)count; // calculates the average but getting the sum and dividing by the number of nodes

	printf("The number of nodes in this list: %d\n", count); fflush(stdout); // prints the number of node in list
	printf("The maximum count in this list: %d\n", max); fflush(stdout); // prints the maximum value in list
	printf("The minimum count in this list: %d\n", min); fflush(stdout); // prints the minimum value in list
	printf("The average count in this list: %f\n", avg); fflush(stdout); // prints the average count in the list

	return;
} // end of print statistics function

//print list function
//prints every node in the list with the corresponding symbol and count
void pList(const PTRNODE list) // this function will print the nodes of the list
{
	PTRNODE head = list; // declaring the head
	if (head == NULL) // list will print empty if the head is null
	{
		printf("The list is empty.\n"); fflush(stdout);
	}
	else // if the list is not empty
	{
		printf("\n"); fflush(stdout);
		while (head != NULL) // while loop that goes through the list until it reaches null
		{
			printf("Symbol: %s Count: [%d]\n", head->symbol, head->count); fflush(stdout); // prints out the symbol and count of each node
			head = head->next; // iterate to the next node in the list
		}
	}
	return;
} // end of the print list function
// Nayab Kaurejo
// This function prints the nodes in the list between two given integer values
void pRange(int a, int b, const PTRNODE list) 
{
	PTRNODE head = list; // declaring the head from the pointer passed from the function
	if (head == NULL) // if head is null the program will print empty and stop
	{
		printf("The list is empty."); fflush(stdout);
	}
	printf("\n"); fflush(stdout);
	while (head != NULL)//step through the list until the NULL pointer at the end.
	{
		if (a <= head->count && b >= head->count) // goes through the list count between the two values given
		{
			printf("Symbol: %s Count: %d\n", head->symbol, head->count); fflush(stdout); // prints out the symbol and count 
		}
		head = head->next; // iterates through the list
	}
	return;
} // end of the print range function

//Nayab Kaurejo
// This function prints all of the nodes given a prefix entered by the console
void pPrefix(char *s, const PTRNODE list) // this function will print node in the list if they match a nodes prefix
{
	PTRNODE head = list; // declaring the head
	if (head == NULL) // if head is null the program will print empty and stop
	{
		printf("The list is empty."); fflush(stdout);
	}
	printf("\n"); fflush(stdout);
	while (head != NULL)//step through the list until the NULL pointer at the end.
	{
		if (!strncmp(head->symbol, s, strlen(s))) // this function compares the symbol of the node up to the string length s
		{
			printf("Symbol: %s Count: %d\n", head->symbol, head->count); fflush(stdout); // prints out each node which has the prefix
		}
		head = head->next; // goes through the list until the end
	}
	return;
} // ending the print prefix function

//Print Suffix function - Greg O'Connor
//Prints all the nodes that have the suffix given by String s
void pSuffix(char *s, const PTRNODE list)
{
	int i, j, match; //i and j are counters, match is a boolean holder
	PTRNODE current; //current node is used to scrub through the list
	current = list;

	printf("\n"); fflush(stdout);
	while(current != NULL) //step through the list until the NULL pointer at the end.
	{
		match = 1; //default to "true"
		for(i = 0; current->symbol[i] != '\0'; i++){} //find the end of the node's symbol
		for(j = 0; s[j] != '\0'; j++){} //find the end of the suffix

		while(j >= 0 && i >= 0) //until either i or j or both are 0
		{
			if(current->symbol[i--] != s[j--] && match)//if two characters are not the same, set match to "false"
			{
				match = 0; //and by "false," I mean zero.
			}
		}
		if(match)//if match isn't zero, output the information of this node.
		{
			printf("[%s] count: [%d]\n", current->symbol, current->count); fflush(stdout);
		}
		current = current->next; //step to the next node.
	}
	return;//done.
}//end print suffix
