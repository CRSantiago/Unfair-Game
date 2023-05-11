//Christopher Santiago
//Dr. Andrew Steinberg
//COP3502 Computer Science 1
//Programming Assignment 5


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 2000

typedef struct{
	char * name; //dynamic string
	int rank;
}player_t;

//function prototype(s)
player_t* scanRoster(player_t *roster); //read players.txt and populate unsorted roster
void sortRoster(player_t* roster, int p, int r); //quick sort implentation of roster
int partition(player_t* roster, int p, int r); // helper partition function for quick sort
player_t * populateTeam(player_t* roster, player_t *team, int teamNum); //populate teams from sorted roster
double getAverage(player_t * team); //calculate avarage rank of each team
void cleanUp(player_t* roster, char flag); //dealocate memory from roster and teams

int main(void)
{
	int seed;
	printf("Enter seed: ");
	scanf("%d", &seed);
	srand(seed);
	
	player_t *roster = (player_t*) malloc(sizeof(player_t) * MAX);
	player_t *team1 = (player_t*) malloc(sizeof(player_t) * MAX / 2);
	player_t *team2 = (player_t*) malloc(sizeof(player_t) * MAX / 2);
	
	roster = scanRoster(roster);
  sortRoster(roster, 0, MAX - 1);
  
	team1 = populateTeam(roster, team1, 1);
  team2 = populateTeam(roster, team2, 2);
  
	double average1 = getAverage(team1);
	double average2 = getAverage(team2);
	
	printf("Team 1 Rank Average is: %f\n", average1);
	printf("Team 2 Rank Average is: %f\n", average2);

  cleanUp(roster, 'r');
  cleanUp(team1, 't');
  cleanUp(team2, 't');
	
	return 0;
}


player_t* scanRoster(player_t *roster)
{
	FILE *fptr = fopen("players.txt", "r");
	
	char name[20];
	int index = 0;
	
	while(fscanf(fptr, "%s", name) == 1)
	{
		roster[index].name = (char *) malloc(sizeof(char) * 20);
		strcpy(roster[index].name, name);
		roster[index].rank = rand() % 5 + 1;
		++index;
	}
	
	fclose(fptr);
	
	return roster;
}

void sortRoster(player_t* roster, int p, int r){
  if(p < r){
    int q = partition(roster, p , r);
    sortRoster(roster, p, q - 1);
    sortRoster(roster, q + 1, r);
  }
}

int partition(player_t* roster, int p, int r){
  int x = roster[r].rank;
  int i = (p - 1);

  for(int j = p; j < r; j++){
    if(roster[j].rank <= x){
      ++i;
      //handle duplicate swaps
      if((roster[i].rank != roster[j].rank)){
        player_t * temp = (player_t*) malloc(sizeof(player_t));
        temp->rank = roster[i].rank;
        temp->name = (char *) malloc(sizeof(char) * 20);
        strcpy(temp->name, roster[i].name);

        strcpy(roster[i].name, roster[j].name);
        roster[i].rank = roster[j].rank;

        strcpy(roster[j].name, temp->name);
        roster[j].rank = temp->rank;

        free(temp->name);
        free(temp);
      }
    }
  }
  
  //handle duplicate swaps
  if(roster[i+1].rank != roster[r].rank){
    player_t * temp = (player_t*) malloc(sizeof(player_t));
    temp->rank = roster[i+1].rank;
    temp->name = (char *) malloc(sizeof(char) * 20);
    strcpy(temp->name, roster[i+1].name);

    strcpy(roster[i+1].name, roster[r].name);
    roster[i+1].rank = roster[r].rank;

    strcpy(roster[r].name, temp->name);
    roster[r].rank = temp->rank;

    free(temp->name);
    free(temp);
  }
  
  return (i+1);
}

player_t * populateTeam(player_t* roster,player_t *team, int teamNum){
  int start, end;
  //teamNum used as a flag to determine indexing for team assignment
  if(teamNum == 1){
    start = 0;
    end = ((MAX/2)-1);
  } else {
    start = MAX / 2;
    end = MAX - 1;
  }

  int i = 0;
  while(start <= end){
    team[i].name = (char *) malloc(sizeof(char) * 20);
    strcpy(team[i].name, roster[start].name);
    team[i].rank = roster[start].rank;
    i++;
    start++;
  }
  return team;
}

double getAverage(player_t * team){
  int sum = 0;
  int end = (MAX/2)-1;
  double average;
  for(int i = 0; i <= end; ++i){
    sum += team[i].rank;
  }
  average = (double) sum / (MAX/2);
  return average;
}

void cleanUp(player_t* roster, char flag){
  int end;
  //flag used to determine if a roster or team was passed as argument
  if(flag == 'r'){
    end = MAX - 1;
  } else if(flag == 't'){
    end = (MAX / 2) - 1;
  }

  for(int i = 0; i <= end; ++i){
    free(roster[i].name);
  }
  free(roster);
}