#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

void * supplier(void *);
void * tea_enthusiast(void *);

// Create global array that represents the amount of available pouches for ingredients
// This should be protected by some form of locking mechanism

const int PRODUCERS = 6;
enum ingredients {GREEN, BLACK, BERGAMOT, HIBISCUS, RICE, SPICES};
char * names[6] = {"Green Leaf", "Black Leaf", "Bergamot Oil", "Hibiscus Leaf", "Puffed Rice", "Spices"};
const int CONSUMERS = 14;

// For colored printing
static char * green = "\033[0;92m";
static char * red = "\033[0;31m";
static char * uncolor = "\033[0m";

// Create global mutex (locks) and semaphores
pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER; // prevent output over eachother
sem_t full_sem[6]; // Safely tracks pouches
sem_t empty_sem[6]; // Helps producers wait til empty

// Other global variables
bool supply = true;

int main()
{
    srand(time(NULL)); // Initialize random seed

    // Display Header
    printf("Tea Enthusiast Producer/Consumer Problem - Solved by CollateralJ\n");

    // Initializers
    for(int i = 0; i < PRODUCERS; i++)
    {
        sem_init(&full_sem[i], 0, 0); // Initialize slots to empty
        sem_init(&empty_sem[i], 0, 0); // binary sem
    }

    // Create 6 supplier threads
    pthread_t producers[PRODUCERS];
    for(int i = 0; i < PRODUCERS; i++)
    {
        pthread_create(&producers[i], NULL, supplier, (void *)(long)i);
    }

    // Create 14 tea enthusiast threads
    pthread_t consumers[CONSUMERS];
    for(int i = 0; i < CONSUMERS; i++)
    {
        pthread_create(&consumers[i], NULL, tea_enthusiast, (void *)(long)i);
    }

    // Wait for enthusiasts to finish running
    for(int i = 0; i < CONSUMERS; i++)
    {
        pthread_join(consumers[i], NULL);
    }
    printf("Tea enthusiasts have finished running\n");

    // Tell the suppliers their job is done and wait for them to exit

    supply = false;
    for(int i = 0; i < PRODUCERS; i++)
    {
        sem_post(&empty_sem[i]);
        pthread_join(producers[i], NULL);
    }
    printf("Suppliers finished running\n");

    // Destroy locks, cond variables (technically unnecessary)
    for(int i = 0; i < PRODUCERS; i++)
    {
        sem_destroy(&full_sem[i]); // Initialize slots to empty
        sem_destroy(&empty_sem[i]); // binary sem
    }
    pthread_mutex_destroy(&print_lock);

    printf("Program Complete\n");

    return 0;
}

/* producer duties:
initialize:
- Choose a number 1-10 for a max amount to keep in pouch
loop:
- Add max amount to pouch to available pouch of ingredient, and print red
- Signal to waiting consumers
- Sleep until pouches reaches 0
*/

void * supplier(void * ing)
{
    long ingredient = (long)ing;
    int pouch_size = (rand() % 10) + 1;

    // loop
    while(supply)
    {
        int amount = 0;
        sem_getvalue(&full_sem[ingredient], &amount);
        if(amount <= 0)
        {
            // - Add max amount to pouch to available pouch of ingredient, and print red
            for(int i = 0; i < pouch_size; i++)
            {
                sem_post(&full_sem[ingredient]);
            }
            pthread_mutex_lock(&print_lock);
            printf("%s%s supplier added %d pouches.%s\n", red, names[ingredient], pouch_size, uncolor);
            pthread_mutex_unlock(&print_lock);
        }

        // - Sleep until pouch changes
        sem_wait(&empty_sem[ingredient]);
        
    }

    pthread_mutex_lock(&print_lock);
    printf("%s%s supplier is done%s\n", red, names[ingredient], uncolor);
    pthread_mutex_unlock(&print_lock);

    pthread_exit(NULL);

    return NULL;
}

/* Consumer duties:
loop 15-25 times:
- Decide how many varieties of tea leaf to use (1-2)
- Pick which variety(s) (black or green)
- Decide how many ingredients (1-4)
- Pick which ingredient(s) (bergamot oil, hibiscus leaves, puffed rice, or spices)
- Print the chosen recipe

- Loop in order of chosen ingredients until we have every ingredient, decrementing as we go
- Print the received tea
- Drink the tea
*/

void * tea_enthusiast(void * thread_id)
{
    long id = (long)thread_id; // Convert arg
    int loops = (rand() % 11) + 15;
    for(int i = 0; i < loops; i++)
    {
        // - Decide how many varieties of tea leaf to use (1-2)
        int varieties = rand() % 2;
        varieties++;

        // - Decide how many ingredients (1-4)
        int ingredient_count = rand() % 4;
        ingredient_count++;

        // - Pick which variety(s) (black or green)
        bool recipe[6] = {0, 0, 0, 0, 0, 0};
        if (varieties == 2)
        {
            // Add both
            recipe[GREEN] = 1;
            recipe[BLACK] = 1;
        } else
        {
            // Add random one
            recipe[rand() % 2] = 1;
        }

        // - Pick which ingredient(s) (bergamot oil, hibiscus leaves, puffed rice, or spices)
        for(int i = 0; i < ingredient_count;)
        {
            int ingredient = (rand() % 4) + 2;
            // Add random one
            if(recipe[ingredient] == 0)
                i++;
            recipe[ingredient] = 1;
        }

        // - Print the chosen recipe
        pthread_mutex_lock(&print_lock);
        printf("%sEnthusiast %ld requesting tea with ingredients:\n        ", green, id);
        bool printed = 0;
        for(int i = 0; i < 6; i++)
        {
            if(recipe[i])
            {
                if (printed)
                    printf(", ");
                printed = 1;
                printf("%s", names[i]);
            }
        }
        printf("%s\n", uncolor);
        pthread_mutex_unlock(&print_lock);

        // - Loop in order of chosen ingredients until we have every ingredient we need
        for(int i = 0; i < 6; i++)
        {
            // Check if we need current ingredient, then obtain it
            if (recipe[i])
            {
                sem_wait(&full_sem[i]); // take
                sem_post(&empty_sem[i]); // signal taken
            }
        }

        // - Print the received tea
        pthread_mutex_lock(&print_lock);
        printf("%s> Enthusiast %ld has received their tea%s\n", green, id, uncolor);
        pthread_mutex_unlock(&print_lock);

        // - Drink the tea
        usleep(rand() % 5000);
    }

    pthread_mutex_lock(&print_lock);
    printf("%sEnthusiast %ld is done%s\n", green, id, uncolor);
    pthread_mutex_unlock(&print_lock);

    pthread_exit(NULL);
    
    return NULL;
}