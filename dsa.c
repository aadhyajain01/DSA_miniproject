#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEATS 20

struct Movie
{
    int id;
    char name[50];
    float rating;
    int seats[MAX_SEATS];
};

struct Movie movies[3] =
{
    {1, "Avengers Endgame", 8.4, {0}},
    {2, "Interstellar", 8.7, {0}},
    {3, "Inception", 8.8, {0}}
};

struct Booking
{
    int bookingID;
    char name[50];
    int movieID;
    int seatNo;

    struct Booking *next;
};

struct Booking *head = NULL;

struct Queue
{
    char name[50];
    int movieID;
};

struct Queue waiting[MAX_SEATS];

int front = -1;
int rear = -1;

void displayMovies();
void displaySeats(int movieID);
void bookTicket();
void cancelTicket();
void displayBookings();

void addBooking(int id, char name[], int movieID, int seat);

void enqueue(char name[], int movieID);
void displayQueue();

int main()
{
    int choice;

    while(1)
    {
        printf("\n\n====================================");
        printf("\n     MOVIE TICKET BOOKING SYSTEM");
        printf("\n====================================");

        printf("\n1. Display Movies");
        printf("\n2. Display Seats");
        printf("\n3. Book Ticket");
        printf("\n4. Cancel Ticket");
        printf("\n5. Display Bookings");
        printf("\n6. Display Waiting Queue");
        printf("\n7. Exit");

        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                displayMovies();
                break;

            case 2:
            {
                int movieID;

                displayMovies();

                printf("\nEnter Movie ID: ");
                scanf("%d", &movieID);

                if(movieID < 1 || movieID > 3)
                {
                    printf("\nInvalid Movie ID!");
                }
                else
                {
                    displaySeats(movieID - 1);
                }

                break;
            }

            case 3:
                bookTicket();
                break;

            case 4:
                cancelTicket();
                break;

            case 5:
                displayBookings();
                break;

            case 6:
                displayQueue();
                break;

            case 7:
                printf("\nThank you for using Movie Ticket Booking System!\n");
                exit(0);

            default:
                printf("\nInvalid choice!");
        }
    }

    return 0;
}

void displayMovies()
{
    printf("\n\n------------- MOVIES -------------\n");

    for(int i = 0; i < 3; i++)
    {
        printf("%d. %s | Rating: %.1f\n",
               movies[i].id,
               movies[i].name,
               movies[i].rating);
    }
}

void displaySeats(int movieID)
{
    printf("\n\n------------- SEATS -------------");
    printf("\nMovie: %s\n", movies[movieID].name);

    for(int i = 0; i < MAX_SEATS; i++)
    {
        if(movies[movieID].seats[i] == 0)
        {
            printf("\nSeat %d : Available", i + 1);
        }
        else
        {
            printf("\nSeat %d : Booked", i + 1);
        }
    }
}

void bookTicket()
{
    int movieID;
    int seatNo;
    char name[50];
    int bookingID;

    displayMovies();

    printf("\nEnter Movie ID: ");
    scanf("%d", &movieID);

    if(movieID < 1 || movieID > 3)
    {
        printf("\nInvalid Movie ID!");
        return;
    }

    displaySeats(movieID - 1);

    printf("\n\nEnter Seat Number: ");
    scanf("%d", &seatNo);

    if(seatNo < 1 || seatNo > MAX_SEATS)
    {
        printf("\nInvalid Seat Number!");
        return;
    }

    if(movies[movieID - 1].seats[seatNo - 1] == 1)
    {
        printf("\nSeat already booked!");

        printf("\nWould you like to join waiting queue?");
        printf("\nEnter your name: ");
        scanf("%s", name);

        enqueue(name, movieID);

        return;
    }

    printf("\nEnter Customer Name: ");
    scanf("%s", name);

    printf("Enter Booking ID: ");
    scanf("%d", &bookingID);

    movies[movieID - 1].seats[seatNo - 1] = 1;

    addBooking(bookingID, name, movieID, seatNo);

    printf("\n\nTicket booked successfully!");

    printf("\nMovie : %s", movies[movieID - 1].name);
    printf("\nSeat  : %d", seatNo);
}

void addBooking(int id, char name[], int movieID, int seat)
{
    struct Booking *newNode;

    newNode = (struct Booking*)malloc(sizeof(struct Booking));

    if(newNode == NULL)
    {
        printf("\nMemory allocation failed!");
        return;
    }

    newNode->bookingID = id;
    strcpy(newNode->name, name);
    newNode->movieID = movieID;
    newNode->seatNo = seat;
    newNode->next = NULL;

    if(head == NULL)
    {
        head = newNode;
    }
    else
    {
        struct Booking *temp = head;

        while(temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = newNode;
    }
}

void displayBookings()
{
    struct Booking *temp = head;

    if(temp == NULL)
    {
        printf("\nNo bookings available.");
        return;
    }

    printf("\n\n------------- BOOKINGS -------------");

    while(temp != NULL)
    {
        printf("\n\nBooking ID : %d", temp->bookingID);
        printf("\nCustomer   : %s", temp->name);
        printf("\nMovie      : %s",
               movies[temp->movieID - 1].name);
        printf("\nSeat No.   : %d", temp->seatNo);

        temp = temp->next;
    }
}

void cancelTicket()
{
    int id;

    printf("\nEnter Booking ID to cancel: ");
    scanf("%d", &id);

    struct Booking *temp = head;
    struct Booking *prev = NULL;

    while(temp != NULL)
    {
        if(temp->bookingID == id)
        {
            movies[temp->movieID - 1]
                .seats[temp->seatNo - 1] = 0;

            if(prev == NULL)
            {
                head = temp->next;
            }
            else
            {
                prev->next = temp->next;
            }

            free(temp);

            printf("\nTicket cancelled successfully!");

            return;
        }

        prev = temp;
        temp = temp->next;
    }

    printf("\nBooking not found!");
}

void enqueue(char name[], int movieID)
{
    if(rear == MAX_SEATS - 1)
    {
        printf("\nWaiting queue is full!");
        return;
    }

    if(front == -1)
    {
        front = 0;
    }

    rear++;

    strcpy(waiting[rear].name, name);
    waiting[rear].movieID = movieID;

    printf("\nCustomer added to waiting queue!");
}

void displayQueue()
{
    if(front == -1 || front > rear)
    {
        printf("\nWaiting queue is empty!");
        return;
    }

    printf("\n\n------------- WAITING QUEUE -------------");

    for(int i = front; i <= rear; i++)
    {
        printf("\n%d. %s | Movie: %s",
               i - front + 1,
               waiting[i].name,
               movies[waiting[i].movieID - 1].name);
    }
}










