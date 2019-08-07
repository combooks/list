OBJECTS = list.o
TARGET = test
CC = gcc
CFLAGS = -W -Wall -Wextra -Werror -Wmissing-prototypes -g

$(TARGET):$(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

.c.o:
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f $(TARGET) $(OBJECTS)
