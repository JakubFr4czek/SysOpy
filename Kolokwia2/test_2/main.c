/**
 * W tym programie mamy dwa wątki: producenta i konsumenta. Wątek producenta produkuje 
 * wartości i przechowuje je w współdzielonym buforze, podczas gdy wątek konsumenta 
 * pobiera wartości z bufora.
 * 
 * Używamy muteksu i dwóch zmiennych warunkowych do synchronizacji dostępu do bufora. 
 * Muteks zapewnia, że tylko jeden wątek może jednocześnie uzyskać dostęp do bufora, 
 * podczas gdy zmienne warunkowe pozwalają wątkom sygnalizować sobie nawzajem, gdy bufor 
 * jest pełny lub pusty. 
 * 
 * Wątek producenta zajmuje muteks, sprawdza za pomocą zmiennej count, czy bufor jest 
 * pełny, i jeśli tak, oczekuje na zmiennej warunkowej producer_cond. Gdy w buforze jest 
 * miejsce, produkuje wartość, przechowuje ją w buforze i sygnalizuje zmiennej warunkowej, 
 * aby powiadomić wątek konsumenta. Na koniec zwalnia muteks.
 * 
 * Wątek konsumenta zajmuje muteks, sprawdza za pomocą zmiennej count, czy bufor jest 
 * pusty, i jeśli tak, oczekuje na zmiennej warunkowej consumer_cond. Gdy wartość jest 
 * dostępna w buforze, pobiera ją, przetwarza wartość i sygnalizuje zmiennej warunkowej 
 * producer_cond, aby powiadomić wątek producenta. Na koniec zwalnia muteks.
 * 
 * Funkcja main tworzy wątki producenta i konsumenta, a następnie oczekuje na ich 
 * zakończenie.
*/
#include "checks.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 10
#define VALUE_COUNT 20

int buffer[BUFFER_SIZE];
int count = 0;


void *producer(void *arg)
{
    /* Podaj stworzony przez siebie, odpowiedni muteks do funkcji check_mutex_producer */
    check_mutex_producer(&...);
    /* Podaj stworzoną przez siebie, odpowiednią zmienną warunkową do funkcji check_cond_producer */
    check_cond_producer(&...);

    int i;
    for (i = 0; i < VALUE_COUNT; ++i)
    {
        /* Zajmij mutex */

        /* Tu czekaj tak długo jak: count == BUFFER_SIZE */

        buffer[count++] = i;

        /* Zasygnalizuj, że bufor jest pełny */

        /* Zwolnij mutex */
    }

    return NULL;
}

void *consumer(void *arg)
{
    /* Podaj stworzony przez siebie, odpowiedni muteks do funkcji check_mutex_consumer */
    check_mutex_consumer(&...);
    /* Podaj stworzoną przez siebie, odpowiednią zmienną warunkową do funkcji check_cond_consumer */
    check_cond_consumer(&...);

    int i;
    for (i = 0; i < VALUE_COUNT; ++i)
    {
        /* Zajmij mutex */

        /* Tu czekaj tak długo jak: count == 0 */

        int value = buffer[--count];
        process_value(value);

        /* Zasygnalizuj, że bufor jest pusty */

        /* Zwolnij mutex */
    }

    return NULL;
}

int main()
{
    /* Stwórz wątek producenta, podaj mu funkcję producer() do wykonywania */ 


    /* Stwórz wątek konsumenta, podaj mu funkcję consumer() do wykonywania */


    /* Poczekaj na zakończenie działania wątków */


    check_wait();
    check_results();

    return 0;
}
