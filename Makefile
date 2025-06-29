CC ?= clang

VERSION := 0.1.0
BINDIR  := bin
BINARY  := service # change this
LDFLAGS := -lulfius -ljansson -lorcania
CFLAGS  := -Dgit_sha=$(shell git rev-parse HEAD)

$(BINDIR)/$(BINARY): $(BINDIR) clean
	$(CC) logger.c http.c config.c api.c main.c $(CFLAGS) -o $@ $(LDFLAGS)

$(BINDIR):
	mkdir -p $@

.PHONY: push
push:
	docker push briandowns/$(BINARY):latest

.PHONY:
clean:
	rm -f $(BINDIR)/*