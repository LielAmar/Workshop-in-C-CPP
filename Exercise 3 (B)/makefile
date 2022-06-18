.PHONY: tweets, snakes

EXTRA = markov_chain.o linked_list.o
TWEETS = tweets_generator.c $(EXTRA)
SNAKES = snakes_and_ladders.c $(EXTRA)
CCFLAGS = -Wall -Wextra -Wvla
CC = gcc

tweets: $(TWEETS)
	$(CC) $^ -o tweets_generator

snakes: $(SNAKES)
	$(CC) $^ -o snakes_and_ladders

markov_chain.o: markov_chain.c markov_chain.h
	$(CC) $(CCFLAGS) -c $<

linked_list.o: linked_list.c linked_list.h
	$(CC) $(CCFLAGS) -c $^

tweets_generator.o: tweets_generator.c tweets_generator.h
	$(CC) $(CCFLAGS) -c $^

snakes_and_ladders.o: snakes_and_ladders.c snakes_and_ladders.h
	$(CC) $(CCFLAGS) -c $^