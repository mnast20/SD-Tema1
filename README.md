# SD-Tema1

The galaxy is a doubly circular list consisting of "nodes" or planets (struct Node). Every planet contains data, some information about it (struct Planet), for example its name, its number of shields, number of planets destroyed and the list of shields (struct Node).  

The function "Get_Node" is used to locate the planet at given index in the galaxy.  
The function "Get_Shield" finds the requested shield in the data of the planet (struct Planet).   

## ADD command:
### "Add_Planet" function
A planet is added to the list and the "data" field of the node is allocated and becomes a pointer to a struct Planet that contains the name of the planet, its number of shields, number of planets destroyed and the list of shields. For the shields, a "Planet_Add_Shield_Last" function is used to initialize them.

## BLH command:
### "Black_Hole" function
A function to remove the node ("Remove_Planet") is used to cut the links to the other planets and replace the deleted one with the next in line. In additionto that, the memory allocated for the list of shields and its contents is freed and so is the planet data.

## UPG command: 
### "Upgrade_Shield" function
This function assigns new value to wanted shield by using memcpy.

## EXP command:
### "Expand" function
The "Planet_Add_Shield_Last" function is called and assigns given value to the new last shield of the specified planet.

## RMV command:
### "Remove_Shield" function
This command removes the shield at given index of a certain planet. It cuts the links to other shields and frees the data allocated for  the removed shield and its value.

## COL command:
### "Collision" function
After the collision of the 2 specific shields, if either one of the both values become -1, then the "Remove_Planet" function is called to delete the planet that lost its shield and as a result, got destroyed.

## ROT command:
### "Rotate" function
In case of a trigonometrical rotation, a permutation to the left is performed. However, if a counter trigonometrical rotation is requested, a permutation to the right is used. 
		
## SHW command:
### Show_Planet function
This function prints the information assigned to a planet.

In the end, the memory allocated for the galaxy, each planet, its data, their shields and shield data is freed ("free_galaxy", "free_planet" functions).

# Link to project:
https://ocw.cs.pub.ro/courses/sd-ca/teme/tema1-2021
