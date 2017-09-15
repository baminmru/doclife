.AUTODEPEND

#		*Translator Definitions*
CC = bcc +DOC.CFG
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH = C:\BC\LIB
INCLUDEPATH = C:\BC\INCLUDE


#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

#		*List Macros*


EXE_dependencies =  \
 bbg.obj \
 bbg1.obj \
 bbg2.obj \
 btrv.obj \
 builder.obj \
 buildf25.obj \
 bytecell.obj \
 cell.obj \
 choose.obj \
 clsbcell.obj \
 clsrcell.obj \
 clsview.obj \
 colorkit.obj \
 date.obj \
 datecell.obj \
 dbf.obj \
 dblcell.obj \
 desktop.obj \
 digcell.obj \
 doc01.obj \
 doc02.obj \
 doc03.obj \
 doc04.obj \
 doc05.obj \
 endofp.obj \
 event.obj \
 expert.obj \
 f10.obj \
 f11.obj \
 f12.obj \
 f13.obj \
 f14.obj \
 f15.obj \
 f16.obj \
 f17.obj \
 f18.obj \
 f19.obj \
 f20.obj \
 f21.obj \
 f22.obj \
 f23.obj \
 f24.obj \
 f25.obj \
 f26.obj \
 f27.obj \
 f28.obj \
 f29.obj \
 f30.obj \
 fltcell.obj \
 fview.obj \
 gauge.obj \
 help.obj \
 hmenu.obj \
 key.obj \
 ldblcell.obj \
 list.obj \
 logo.obj \
 lowprint.obj \
 main.obj \
 meswin.obj \
 mouse.obj \
 mynew.obj \
 mystring.obj \
 num2str.obj \
 parus.obj \
 password.obj \
 protect.obj \
 queue.obj \
 relccell.obj \
 rellcell.obj \
 relscell.obj \
 screen.obj \
 shrtcell.obj \
 styles.obj \
 timecell.obj \
 trace.obj \
 vmenu.obj \
 window.obj \
 yncell.obj

#		*Explicit Rules*
doc.exe: doc.cfg $(EXE_dependencies)
  $(TLINK) /x/c/P-/L$(LIBPATH)/o @&&|
/o- c0l.obj+
bbg.obj+
bbg1.obj+
bbg2.obj+
btrv.obj+
builder.obj+
/o+ buildf25.obj+
/o- bytecell.obj+
cell.obj+
/o+ choose.obj+
/o- clsbcell.obj+
clsrcell.obj+
clsview.obj+
/o+ colorkit.obj+
/o- date.obj+
datecell.obj+
/o+ dbf.obj+
/o- dblcell.obj+
desktop.obj+
digcell.obj+
/o+ doc01.obj+
doc02.obj+
doc03.obj+
doc04.obj+
doc05.obj+
endofp.obj+
/o- event.obj+
/o+ expert.obj+
f10.obj+
f11.obj+
f12.obj+
f13.obj+
f14.obj+
f15.obj+
f16.obj+
f17.obj+
f18.obj+
f19.obj+
f20.obj+
f21.obj+
f22.obj+
f23.obj+
f24.obj+
f25.obj+
f26.obj+
f27.obj+
f28.obj+
f29.obj+
f30.obj+
/o- fltcell.obj+
fview.obj+
gauge.obj+
/o+ help.obj+
/o- hmenu.obj+
key.obj+
ldblcell.obj+
list.obj+
/o+ logo.obj+
lowprint.obj+
/o- main.obj+
meswin.obj+
mouse.obj+
mynew.obj+
mystring.obj+
/o+ num2str.obj+
parus.obj+
password.obj+
protect.obj+
/o- queue.obj+
relccell.obj+
rellcell.obj+
relscell.obj+
screen.obj+
shrtcell.obj+
styles.obj+
timecell.obj+
trace.obj+
vmenu.obj+
window.obj+
yncell.obj
doc
		# no map file
overlay.lib+
emu.lib+
mathl.lib+
cl.lib
|


#		*Individual File Dependencies*
bbg.obj:  bbg.cpp

bbg1.obj:  bbg1.cpp

bbg2.obj:  bbg2.cpp

btrv.obj:  btrv.cpp

builder.obj:  builder.cpp

buildf25.obj:  buildf25.cpp

bytecell.obj:  bytecell.cpp

cell.obj:  cell.cpp

choose.obj:  choose.cpp

clsbcell.obj:  clsbcell.cpp

clsrcell.obj:  clsrcell.cpp

clsview.obj:  clsview.cpp

colorkit.obj:  colorkit.cpp

date.obj:  date.cpp

datecell.obj:  datecell.cpp

dbf.obj:  dbf.cpp

dblcell.obj:  dblcell.cpp

desktop.obj:  desktop.cpp

digcell.obj:  digcell.cpp

doc01.obj:  doc01.cpp

doc02.obj:  doc02.cpp

doc03.obj:  doc03.cpp

doc04.obj:  doc04.cpp

doc05.obj:  doc05.cpp

endofp.obj:  endofp.cpp

event.obj:  event.cpp

expert.obj:  expert.cpp

f10.obj:  f10.cpp

f11.obj:  f11.cpp

f12.obj:  f12.cpp

f13.obj:  f13.cpp

f14.obj:  f14.cpp

f15.obj:  f15.cpp

f16.obj:  f16.cpp

f17.obj:  f17.cpp

f18.obj:  f18.cpp

f19.obj:  f19.cpp

f20.obj:  f20.cpp

f21.obj:  f21.cpp

f22.obj:  f22.cpp

f23.obj:  f23.cpp

f24.obj:  f24.cpp

f25.obj:  f25.cpp

f26.obj:  f26.cpp

f27.obj:  f27.cpp

f28.obj:  f28.cpp

f29.obj:  f29.cpp

f30.obj:  f30.cpp

fltcell.obj:  fltcell.cpp

fview.obj:  fview.cpp

gauge.obj:  gauge.cpp

help.obj:  help.cpp

hmenu.obj:  hmenu.cpp

key.obj:  key.cpp

ldblcell.obj:  ldblcell.cpp

list.obj:  list.cpp

logo.obj:  logo.cpp

lowprint.obj:  lowprint.cpp

main.obj:  main.cpp

meswin.obj:  meswin.cpp

mouse.obj:  mouse.cpp

mynew.obj:  mynew.cpp

mystring.obj:  mystring.cpp

num2str.obj:  num2str.cpp

parus.obj:  parus.cpp

password.obj:  password.cpp

protect.obj:  protect.cpp

queue.obj:  queue.cpp

relccell.obj:  relccell.cpp

rellcell.obj:  rellcell.cpp

relscell.obj:  relscell.cpp

screen.obj:  screen.cpp

shrtcell.obj:  shrtcell.cpp

styles.obj:  styles.cpp

timecell.obj:  timecell.cpp

trace.obj:  trace.cpp

vmenu.obj:  vmenu.cpp

window.obj:  window.cpp

yncell.obj:  yncell.cpp

#		*Compiler Configuration File*
doc.cfg: doc.mak
  copy &&|
-ml
-v
-Y
-vi-
-wpro
-weas
-wpre
-I$(INCLUDEPATH)
-L$(LIBPATH)
| doc.cfg


