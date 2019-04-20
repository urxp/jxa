JXA_FLAGS = -O3 -m32
JXA_I = -I./include

JXA_AR = llvm-ar-8
JXA_CC = clang-8
JXA_LD = ld.lld-8

LIB_NAME = jxa

NAMES = jxa json_parse json_interface json_find json_free json_print

H_FILES = src/jxa.h include/json.h
STATIC_O_FILES = $(addprefix o/static/, $(addsuffix .o, $(NAMES)))
SHARED_O_FILES = $(addprefix o/shared/, $(addsuffix .o, $(NAMES)))





all: static shared


lib/lib$(LIB_NAME).a: $(STATIC_O_FILES)
	$(JXA_AR) rcs $@ $?

o/static/%.o: src/%.c $(H_FILES)
	$(JXA_CC) -c $< -o $@ $(JXA_FLAGS) $(JXA_I)


lib/lib$(LIB_NAME).so: $(SHARED_O_FILES)
	$(JXA_LD) -s -shared -o $@ $^

o/shared/%.o: src/%.c $(H_FILES)
	$(JXA_CC) -fPIC -c $< -o $@ $(JXA_FLAGS) $(JXA_I)





.PHONY: clean dirs \
		msg_static dir_static static \
		msg_shared dir_shared shared

OUT_DIR = o lib
dirs: $(OUT_DIR)
$(OUT_DIR):
	mkdir -p $(OUT_DIR)
dir_static: o/static
dir_shared: o/shared
o/%:
	mkdir -p $@

static: dirs msg_static dir_static lib/lib$(LIB_NAME).a 
msg_static:
	$(info ***** Creating static library... lib$(LIB_NAME).a)
shared: dirs msg_shared dir_shared lib/lib$(LIB_NAME).so 	
msg_shared:
	$(info ***** Creating shared library... lib$(LIB_NAME).so)

clean:
	@rm -r $(OUT_DIR)
