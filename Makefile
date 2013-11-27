ifeq ($(ENABLE_FORKING),1)
	OTHER_FLAGS := $(OTHER_FLAGS) -DENABLE_FORKING
endif
ifeq ($(ENABLE_THREADING),1)
	OTHER_FLAGS := $(OTHER_FLAGS) -DENABLE_THREADING
endif
ifeq ($(ENABLE_DAEMON),1)
	OTHER_FLAGS := $(OTHERFlags) -DENABLE_DAEMON
endif

echo-server: echo-server.c
	gcc $(OTHER_FLAGS) -Wall -Wextra -o $@ $^

.PHONY: clean

clean:
	rm -f echo-server
