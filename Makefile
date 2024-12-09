# 375390	Alexander Aoun
# 371946	Ismail Mostafa

OUT = projet
CXX = g++
CXXFLAGS = -Wall -std=c++17
LINKING = `pkg-config --cflags gtkmm-4.0`
LDLIBS = `pkg-config --libs gtkmm-4.0`
CXXFILES = graphic.cc shape.cc lifeform.cc simulation.cc projet.cc gui.cc \
 message.cc
OFILES = $(CXXFILES:.cc=.o)

all: $(OUT)

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(LINKING) $(CXXFILES)| \
	  egrep -v "/usr" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.cc~ *.h~ $(OUT)
    

graphic.o: graphic.cc graphic_gui.h graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ 

gui.o: gui.cc graphic_gui.h graphic.h gui.h simulation.h lifeform.h \
 shape.h constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ 

projet.o: projet.cc gui.h simulation.h lifeform.h message.h shape.h \
graphic.h constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ 

$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)


# DO NOT DELETE THIS LINE
shape.o: shape.cc shape.h graphic.h
lifeform.o: lifeform.cc message.h lifeform.h constantes.h shape.h \
 graphic.h
simulation.o: simulation.cc message.h simulation.h lifeform.h \
 constantes.h shape.h graphic.h
message.o: message.cc message.h

