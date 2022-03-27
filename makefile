SERVER=server
CLIENTE=cliente

TARGETS.x = agenda.x
TARGETS.h = agenda.h
TARGETS_SVC.c = serverAgenda.c agenda_svc.c agenda_xdr.c
TARGETS_CLNT.c = clienteAgenda.c agenda_clnt.c agenda_xdr.c
TARGETS = agenda.h agenda_svc.c agenda_xdr.c agenda_clnt.c

OBJECTS_CLNT = $(TARGETS_CLNT.c:%.c=%.o)
OBJECTS_SVC = $(TARGETS_SVC.c:%.c=%.o)

RPCGENFLAGS = -C

all : $(TARGETS) $(CLIENTE) $(SERVER)

$(TARGETS) : $(TARGETS.x) 
	rpcgen $(RPCGENFLAGS) $(TARGETS.x)

$(SERVER) : $(OBJECTS_SVC)
	gcc $(TARGETS_SVC.c) -pthread -o $(SERVER)

$(CLIENTE) : $(OBJECTS_CLNT)
	gcc $(TARGETS_CLNT.c) -pthread -o $(CLIENTE)

clean : 
	$(RM) core $(TARGETS) $(OBJECTS_CLNT) $(OBJECTS_SVC) $(CLIENTE) $(SERVER)



