#make file libreria LTG

root = `root-config --cflags --glibs`
lib = -l BASE -l LTG
inc = ./include
src = ./source
obj = ./obj

END: rt pulse ppsys bilayer iv avp avgsys
	@echo OK done!

avp: $(src)/AVP.cpp libLTG.a libBASE.a
	$(CXX) -o $@ $< -I $(inc) -L ./ $(lib) $(root)

avgsys: $(src)/avgsys.cpp libLTG.a
	$(CXX) -o $@ $< -I $(inc) -L ./ $(lib) $(root)

iv: $(src)/iv.cpp libLTG.a libBASE.a
	$(CXX) -o $@ $< -I $(inc) -L ./ $(lib) $(root)

ppsys: $(src)/ppsys.cpp libLTG.a libBASE.a
	$(CXX) -o $@ $< -I $(inc) -L ./ $(lib) $(root)
	@echo ppsys compiled!

pulse: $(src)/PulseConv.cpp libLTG.a libBASE.a
	$(CXX) -o $@ $< -I $(inc) -L ./ $(lib) $(root)
	@echo pulse compiled!

rt: $(src)/rt.cpp libLTG.a libBASE.a
	$(CXX) -o $@ $< -I $(inc) -L ./ $(lib) $(root)
	@echo  rt compiled

bilayer: $(src)/bilayer.cpp libLTG.a libBASE.a
	$(CXX) -o $@ $< -I $(inc) -L ./ $(lib) $(root)
	@echo  bilayer compiled

libLTG.a: $(obj)/rt.ox $(obj)/tools.ox $(obj)/plot.ox $(obj)/iv.ox $(obj)/bilayer.ox
	$(AR) -r $@ $(obj)/*.ox
	@echo Assembled LTG library

libBASE.a: $(obj)/base.o $(obj)/filelist.o $(obj)/iv0.o
	$(AR) -r $@ $(obj)/*.o
	@echo Assembled BASE library

$(obj)/%.ox: $(src)/%.cxx $(inc)/%.h
	mkdir -p $(@D)
	$(CXX) -c $< -o $@ -I $(inc) $(root) 

$(obj)/%.o: $(src)/%.cc $(inc)/%.h
	mkdir -p $(@D)
	$(CXX) -c $< -o $@ -I $(inc)
