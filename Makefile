##########################################################
# 
#  Author:  Dragon, Fong's National Engineering Co., Ltd.
#  Date  :  May 10th, 2009
#  Version: 0.1
#  Remarks: the initification version
# 
##########################################################

target := src/ 

.PHONY: all $(target) 

all: $(target)

$(target):
	$(MAKE) -C $@ $(cmd)
clean:
	rm -rf src/obj/*.o 
	rm -rf fastget
