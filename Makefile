ifeq ($(ENABLE_FORKING),1)
	OTHER_FLAGS := $(OTHER_FLAGS) -DENABLE_FORKING
endif
ifeq ($(ENABLE_THREADING),1)
	OTHER_FLAGS := $(OTHER_FLAGS) -DENABLE_THREADING
endif
ifeq ($(ENABLE_DAEMON),1)
	OTHER_FLAGS := $(OTHER_FLAGS) -DENABLE_DAEMON
endif
ifeq ($(ENABLE_PRIV),1)
	OTHER_FLAGS := $(OTHER_FLAGS) -DENABLE_PRIV
endif

echo-server: echo-server.c
	gcc $(OTHER_FLAGS) -g -Wall -Wextra -o $@ $^

.PHONY: clean

clean:
	rm -f echo-server
