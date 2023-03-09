// Copyright 2021 Nastase Maria
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#define MAX_SIZE 64

typedef struct Node Node;
struct Node
{
	void* data;
	Node *prev, *next;
};

typedef struct Shield Shield;
struct Shield
{
	int value;
	Shield *prev, *next;
};

typedef struct Planet Planet;
struct Planet
{
	int nr_def;
	int destroyed;
	void* name;
	Node *first_shield;
};

typedef struct Galaxy Galaxy;
struct Galaxy
{
	Node* first_planet;
	int data_size;
	int size;
};
Galaxy*
Init_Galaxy(int data_size)
{
	// initialize galaxy
	struct Galaxy* galaxy = malloc(sizeof(struct Galaxy));
	DIE(galaxy == NULL, "Malloc galaxy");
	galaxy->size = 0;
	galaxy->first_planet = NULL;
	galaxy->data_size = data_size;
	return galaxy;
}

Planet* Init_Planet()
{
    // initialize planet
    struct Planet* planet = malloc(sizeof(struct Planet));
    DIE(planet == NULL, "Malloc Planet data");
    planet->name = NULL;
    planet->destroyed = 0;
    planet->first_shield = NULL;
    planet->nr_def = 0;
    return planet;
}

Node*
Get_Node(Galaxy* galaxy, int ind_planet)
{
	// find given node
	struct Node* current_node;
	current_node = galaxy->first_planet;
	for (int i = 0; i < ind_planet; i++) {
		current_node = current_node->next;
	}
	return current_node;
}

Node*
Get_Shield(Planet* planet, int ind_shield)
{
	// find given shield for planet
	struct Node* current_shield;
	current_shield = planet->first_shield;
	for (int i = 0; i < ind_shield; i++) {
		current_shield = current_shield->next;
	}
	return current_shield;
}

void
free_planet(Node** planet)
{
	(*planet)->next = NULL;
	(*planet)->prev = NULL;
	struct Planet* info_planet = (*planet)->data;
    struct Node *current, *tmp;
	current = info_planet->first_shield;
	for (int i = 0; i < info_planet->nr_def; i++)
	{
		// free planet shields
	   tmp = current->next;
       free(current->data);
	   free(current);
	   current = tmp;
	}
	free(info_planet->name);
	free((*planet)->data);
	free(*planet);
}

void Remove_Planet(Galaxy* galaxy, Node* del_planet)
{
	// removes planet from system
	struct Node* tmp;
	if (galaxy->first_planet == NULL || del_planet == NULL)
		return;
	tmp = del_planet->prev;
	if (del_planet == galaxy->first_planet) {
		// planet is the first in the galaxy
		tmp->next = galaxy->first_planet->next;
		galaxy->first_planet->next->prev = tmp;
		galaxy->first_planet = galaxy->first_planet->next;
	} else {
		del_planet = tmp->next;
		tmp->next = del_planet->next;
		del_planet->next->prev = tmp;
	}
	// free removed planet
	free_planet(&del_planet);
	galaxy->size--;
}

void Planet_Add_Shield_Last(Planet *planet, int value)
{
	// adds last shield for given planet
	struct Node *new_shield = malloc(sizeof(struct Node));
	DIE(new_shield == NULL, "Malloc Shield node");
	new_shield->data = malloc(sizeof(int));
    DIE(new_shield->data == NULL, "Malloc Shield data");
    memcpy(new_shield->data, &value, sizeof(int));
	new_shield->next = NULL;
	new_shield->prev = NULL;
	struct Node *current_shield = planet->first_shield;
	if (planet->first_shield == NULL) {
		planet->first_shield = new_shield;
		new_shield->next = new_shield;
		new_shield->prev = new_shield;
	} else {
		current_shield = planet->first_shield->prev;
		new_shield->next = current_shield->next;
		new_shield->prev = current_shield;
		current_shield->next->prev = new_shield;
		current_shield->next = new_shield;
	}
    planet->nr_def++;
}

void Expand(Galaxy* galaxy, int ind_planet, int value)
{
	// EXP command
	struct Node* current_planet = galaxy->first_planet;
	if (ind_planet >= galaxy->size) {
		printf("Planet out of bounds!\n");
		return;
	}
	if (ind_planet != 0) {
		current_planet = Get_Node(galaxy, ind_planet);
	}
	Planet_Add_Shield_Last(current_planet->data, value);
}

void
Add_Planet(Galaxy* galaxy, int ind_planet, int nr_shields, void *name)
{
	// ADD command
	if (galaxy == NULL) {
		return;
	}
	if (ind_planet < 0 || ind_planet > galaxy->size) {
		printf("Planet out of bounds!\n");
		return;
	}
	struct Node *new, *current_planet;
	new = (struct Node*)malloc(sizeof(struct Node));
	DIE(new == NULL, "Malloc Planet node");
	new->data = Init_Planet();
	DIE(new->data == NULL, "Malloc Planet data");
    struct Planet* info_new = new->data;
    info_new->name = malloc(galaxy->data_size);
    DIE(info_new->name == NULL, "Malloc Planet name");
	memcpy(info_new->name, name, galaxy->data_size);
	new->next = NULL;
	new->prev = NULL;
	current_planet = galaxy->first_planet;
	if (ind_planet != 0 && ind_planet != 1) {
		current_planet = Get_Node(galaxy, ind_planet);
	}
	if (galaxy->size == 0) {
		galaxy->first_planet = new;
		new->next = new;
		new->prev = new;
		current_planet = galaxy->first_planet;
	} else {
		if (ind_planet == 0) {
			current_planet = galaxy->first_planet->prev;
			new->next = galaxy->first_planet;
			new->prev = current_planet;
			current_planet->next = new;
			galaxy->first_planet->prev = new;
			galaxy->first_planet = new;
		} else {
			if (ind_planet != 1) {
				current_planet = current_planet->prev;
			}
			new->next = current_planet->next;
			new->prev = current_planet;
			current_planet->next->prev = new;
			current_planet->next = new;
		}
	}
	current_planet = current_planet->next;
    Planet* info_planet = current_planet->data;
	for (int i = 0; i < nr_shields; i++) {
		Planet_Add_Shield_Last(info_planet, 1);
	}
    memcpy(new->data, info_planet, sizeof(Planet));
	printf("The planet %s has joined the galaxy.\n", (char *)info_planet->name);
	galaxy->size++;
}

void Black_Hole(Galaxy* galaxy, int ind_planet)
{
	// BLH command
	struct Node* current_planet = galaxy->first_planet;
	if (ind_planet >= galaxy->size) {
		printf("Planet out of bounds!\n");
		return;
	}
	if (ind_planet != 0) {
		current_planet = Get_Node(galaxy, ind_planet);
	}
    struct Planet* info_planet = current_planet->data;
	printf("The planet %s has been eaten by the vortex.\n",
	(char *)info_planet->name);
	// removes planet eaten by vortex
	Remove_Planet(galaxy, current_planet);
}

void Upgrade_Shield(Galaxy* galaxy, int ind_planet, int ind_shield, int value)
{
	// UPG command
	struct Node* current_planet = galaxy->first_planet;
	if (ind_planet >= galaxy->size) {
		printf("Planet out of bounds!\n");
		return;
	}
	if (ind_planet != 0) {
		current_planet = Get_Node(galaxy, ind_planet);
	}
	struct Planet* info_planet = current_planet->data;
    if (ind_shield >= info_planet->nr_def) {
		printf("Shield out of bounds!\n");
		return;
	}
	struct Node* current_shield = info_planet->first_shield;
	if (ind_shield != 0) {
		// find shield at given index
		current_shield = Get_Shield(info_planet, ind_shield);
	}
	// changes value of given shield
    value += *(int *)current_shield->data;
    memcpy(current_shield->data, &value, sizeof(int));
}

void Remove_Shield(Galaxy* galaxy, int ind_planet, int ind_shield)
{
	// RMV command
	struct Node* current_planet = galaxy->first_planet;
	if (ind_planet >= galaxy->size) {
		printf("Planet out of bounds!\n");
		return;
	}
	if (ind_planet != 0)
		current_planet = Get_Node(galaxy, ind_planet);
	struct Planet* info_planet = current_planet->data;
    if (ind_shield >= info_planet->nr_def) {
		printf("Shield out of bounds!\n");
		return;
	}
	if (info_planet->nr_def == 4) {
		printf("A planet cannot have less than 4 shields!\n");
		return;
	}
	struct Node *current_shield = info_planet->first_shield, *del;
	if (ind_shield != 0 && ind_shield != 1) {
		// find shield that must be deleted
		current_shield = Get_Shield(info_planet, ind_shield);
		current_shield = current_shield->prev;
	}
	if (ind_shield == 0) {
		// shield to be removed is the first one
		current_shield = current_shield->prev;
		del = info_planet->first_shield;
		current_shield->next = info_planet->first_shield->next;
		info_planet->first_shield->next->prev = current_shield;
		info_planet->first_shield = info_planet->first_shield->next;
	} else {
		del = current_shield->next;
		current_shield->next = del->next;
		del->next->prev = current_shield;
	}
	del->next = NULL;
	del->prev = NULL;
    free(del->data);
	free(del);
	info_planet->nr_def--;
}

void Collision(Galaxy* galaxy, int ind_planet1, int ind_planet2) {
	// COL command
	int ok = 0;
	if (ind_planet1 >= galaxy->size || ind_planet2 >= galaxy->size) {
		printf("Planet out of bounds!\n");
		return;
	}
	struct Node *planet1 = galaxy->first_planet, *planet2 = planet1->next;
	if (ind_planet1 != 0) {
		// search the 2 planets that will collide
		planet1 = Get_Node(galaxy, ind_planet1);
		planet2 = planet1->next;
	}
    struct Planet *info_planet1 = planet1->data, *info_planet2 = planet2->data;
	struct Node *shield1 = info_planet1->first_shield,
	*shield2 = info_planet2->first_shield;
	int ind_shield1, ind_shield2;  // indexes of shields that will collide
	ind_shield1 = (info_planet1->nr_def)/4;
	ind_shield2 = 3 * ((info_planet2->nr_def)/4);
	// find the 2 specified shields
	if (ind_shield1 != 0) {
		shield1 = Get_Shield(info_planet1, ind_shield1);
	}
	if (ind_shield2 != 0) {
		shield2 = Get_Shield(info_planet2, ind_shield2);
	}
	// change value of shields
	int val1 = *(int *)shield1->data - 1;
	int val2 = *(int *)shield2->data - 1;
	memcpy(shield1->data, &val1, sizeof(int));
	memcpy(shield2->data, &val2, sizeof(int));
	if (val1 == -1) {
		// shield of planet1 destroyed
		ok = 1;
		info_planet2->destroyed++;
		printf("The planet %s has imploded.\n", (char *)info_planet1->name);
		Remove_Planet(galaxy, planet1);
	}
	if (val2 == -1) {
		// shield of planet2 destroyed
		if (ok == 0) {
			info_planet1->destroyed++;
		}
		printf("The planet %s has imploded.\n", (char *)info_planet2->name);
		Remove_Planet(galaxy, planet2);
	}
}

void rotate_shields_counter(Planet* planet, int units)
{
	// counter trigonometrical rotation
	Node *current_shield = planet->first_shield;
	for (int i = 1; i <= units; i++) {
		// permutation right with nr of units
		current_shield = planet->first_shield->prev;
		int val = *(int *)current_shield->data;
		current_shield = current_shield->prev;
		for (int i = 0; i < planet->nr_def - 1; i++) {
			// change of shield value
			memcpy(current_shield->next->data, current_shield->data, sizeof(int));
			current_shield = current_shield->prev;
		}
		// change of fist shield value
		memcpy(planet->first_shield->data, &val, sizeof(int));
	}
}

void rotate_shields_trigo(Planet* planet, int units)
{
	// trigonometrical rotation
	Node *current_shield = planet->first_shield;
	for (int i = 1; i <= units; i++) {
		// permutation left with nr of units
		int val = *(int*)current_shield->data;
		current_shield = current_shield->next;
		for (int i = 0; i < planet->nr_def - 1; i++) {
			// change of shield value
			memcpy(current_shield->prev->data, current_shield->data, sizeof(int));
			current_shield = current_shield->next;
		}
		// change of last shield value
		memcpy(planet->first_shield->prev->data, &val, sizeof(int));
	}
}

void Rotate(Galaxy* galaxy, int ind_planet, char direction, int units)
{
	// ROT command
	struct Node* current_planet = galaxy->first_planet;
	if (ind_planet >= galaxy->size) {
		printf("Planet out of bounds!\n");
		return;
	}
	if (ind_planet != 0) {
		// find planet
		current_planet = Get_Node(galaxy, ind_planet);
	}
    struct Planet* info_planet = current_planet->data;
	units = units % (info_planet->nr_def);
	if (direction == 't') {
		rotate_shields_trigo(info_planet, units);
	} else if (direction == 'c') {
		rotate_shields_counter(info_planet, units);
	} else {
		printf("Not a valid direction!\n");
	}
}

void free_galaxy(Galaxy** galaxy)
{
	struct Node *current, *tmp;
	current = (*galaxy)->first_planet;
	for (int i = 0; i < (*galaxy)->size; i++)
	{
	   tmp = current->next;
	   free_planet(&current);
	   current = tmp;
	}
	free(*galaxy);
}

void Show_Planet(Galaxy* galaxy, int ind_planet)
{
	// SHW command
	struct Node* current_planet = galaxy->first_planet;
	if (galaxy->size <= ind_planet) {
		printf("Planet out of bounds!\n");
		return;
	}
	if (ind_planet != 0) {
		current_planet = Get_Node(galaxy, ind_planet);
    }
	struct Planet *info_planet = current_planet->data,
    *info_prev = current_planet->prev->data,
    *info_next = current_planet->next->data;
    printf("NAME: %s\n", (char *)info_planet->name);
	printf("CLOSEST: ");
	if (galaxy->size > 2) {
		// display the 2 neighbours
		printf("%s and %s\n",
		(char *)info_prev->name, (char *)info_next->name);
	} else if (galaxy->size == 1) {
		// only 1 planet in galaxy
		printf("none\n");
	} else {
		// 2 planets in galaxy
		printf("%s\n", (char *)info_prev->name);
	}
    printf("SHIELDS: ");
	struct Node* current_shield = info_planet->first_shield;
	for (int i = 0; i < info_planet->nr_def; i++) {
		// display shields of planet
		printf("%d ", *((int*)current_shield->data));
		current_shield = current_shield->next;
	}
	printf("\n");
	printf("KILLED: %d\n", info_planet->destroyed);
}

void Print_galaxy(Galaxy* galaxy)
{
	for (int i = 0; i < galaxy->size; i++) {
		Show_Planet(galaxy, i);
	}

	printf("\n");
}

int main(void) {
	Galaxy *galaxy;
	int M, ind_planet, nr_def, ind_shield, value, ind_planet1, ind_planet2, units;
	char command[1001], name[1001], direction;
	galaxy = Init_Galaxy(MAX_SIZE);
	scanf("%d", &M);
	for (int i = 1; i <= M; i++) {
		scanf("%s", command);
		if (strcmp(command, "ADD") == 0) {
			scanf("%s", name);
			scanf("%d%d", &ind_planet, &nr_def);
			Add_Planet(galaxy, ind_planet, nr_def, name);
		} else if (strcmp(command, "BLH") == 0) {
			scanf("%d", &ind_planet);
			Black_Hole(galaxy, ind_planet);
		} else if (strcmp(command, "UPG") == 0) {
			scanf("%d%d%d", &ind_planet, &ind_shield, &value);
			Upgrade_Shield(galaxy, ind_planet, ind_shield, value);
		} else if (strcmp(command, "EXP") == 0) {
			scanf("%d%d", &ind_planet, &value);
			Expand(galaxy, ind_planet, value);
		} else if (strcmp(command, "RMV") == 0) {
			scanf("%d%d", &ind_planet, &ind_shield);
			Remove_Shield(galaxy, ind_planet, ind_shield);
		} else if (strcmp(command, "COL") == 0) {
			scanf("%d%d", &ind_planet1, &ind_planet2);
			Collision(galaxy, ind_planet1, ind_planet2);
		} else if (strcmp(command, "ROT") == 0) {
			scanf("%d %c %d", &ind_planet, &direction, &units);
			Rotate(galaxy, ind_planet, direction, units);
		} else if (strcmp(command, "SHW") == 0) {
			scanf("%d", &ind_planet);
			Show_Planet(galaxy, ind_planet);
		}
	}
	free_galaxy(&galaxy);
	return 0;
}

