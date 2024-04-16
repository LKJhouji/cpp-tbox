TARGET:=demo

OBJECTS:=01-demo.o

CXXFLAGS:=-I$(HOME)/.tbox/include -DLOG_MODULE_ID='"demo"'
LDFLAGS:=-L$(HOME)/.tbox/lib
LIBS:=\
	-ltbox_main \
	-ltbox_terminal \
	-ltbox_network \
	-ltbox_eventx \
	-ltbox_event \
	-ltbox_log \
	-ltbox_util \
	-ltbox_base \
	-lpthread -ldl

$(TARGET): $(OBJECTS)
	g++ -o $@ $^ $(LDFLAGS) $(LIBS)