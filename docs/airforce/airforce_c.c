 Appendix B - The C Program Code of the Model
// INCLUDED HEADER FILES
#include <time.h> #include <stdio.h> #include <string.h> #include <stdlib.h> #include <malloc.h> #include <conio.h>
// FUNCTION PROTOTYPES
void initialize(void);
void inputboxlist(void);
void execiterations(void);
void listcanditlayers(void);
inteomplayerlist(const void *i, const void *j);
int packlayer(void);
int findlayer( short int thickness);
void flndbox(short int hmx, short int hy, short int hmy, short int hz, short int hmz); void analyzebox (short int hmx, short int hy, short int hmy, short int hz, short int hmz,
void findsmallestz(void);
void checkfound(void);
void volumecheck (void); void graphunpackedout(void); void outputboxlist(void);
void report(void);
short int diml, short int dim2, short int dim3);
B-l
 //****:l;:(:******************** ******************************
// VARIABLE, CONSTANT AND STRUCTURE DECLARATIONS
//********************************************************
char strpx[5], strpy[5], strpz[5], strcox[5], strcoy[5], strcoz[5], strpackx[5], strpacky[5], strpackz[5], filename[12], strtemp[]="", packing, layerdone, evened, variant, bestvariant, packingbest, hundredpercent, graphout[]="visudat", unpacked, quit;
short int tbn, x, n, layerlistlen, layerinlayer, prelayer, lilz, itenum, hour, min, sec, layersindex, remainpx, remainpy, remainpz, packedy, prepackedy, layerthickness, itelayer, boxx, boxy, boxz, boxi, bboxx, bboxy, bboxz, bboxi, preremainpy, cboxi, cboxx, cboxy, cboxz, bfx, bfy, bfz, bbfx, bbfy, bbfz, bestite, packednumbox, bestpackednum, xx, yy, zz, px, py, pz;
double packedvolume, bestvolume, totalvolume, totalboxvol, temp, percentageused, percentagepackedbox, elapsedtime;
struct boxinfo { char packst;
short int diml, dim2, dim3, n, cox, coy, coz, packx, packy, packz;
long int vol; } boxlist[5000];
struct layerlist{
long int layereval;
short int layerdim; } layers[1000];
struct scrappad{
struct scrappad *pre, *pos;
short int cumx, cumz;
};
struct scrappad *scrapfirst, *scrapmemb, *smallestz, *trash; time_t start, finish;
FILE *ifp, *ofp, *gfp;
B-2
 //MAIN PROGRAM
int main(int arge, char *argv[]) { if(argc==l){
printf("(ASSUMEDTOHAVE'.TXT'EXTENSION;UPTO8 CHARACTERS LONG)\n");
printf ("PLEASE ENTER THE NAME OF THE INPUT FILE :"); scanf ("%s",filename);
}
else {
} initialize();
time(&start);
printf("\nPRESS Q TO QUIT AT ANYTIME AND WAIT\n\n"); execiterations();
time(&finish);
reportO;
getch();
return 0;
}
printf("%s", argv[l]); strcpy(filename, argv[l]);
// PERFORMS INITIALIZATIONS
void initialize(void) { if(filename=""){
printf("\nINVALID FILE NAME"); exit(l);
} inputboxlist();
temp=1.0; totalvolume=temp*xx*yy*zz; totalboxvol=0.0;
for (x=l; x<=tbn; x++) totalboxvol=totalboxvol+boxlist[x].vol; scrapfirst=malloc(sizeof(structscrappad));
if ((*scrapfirst).pos==NULL) {
printf("Insufficient memory available\n"); exit(l);
-} (*scrapfirst).pre=NULL;(*scrapfirst).pos=NULL;
bestvolume=0.0; packingbest=0; hundredpercent=0; itenum=0; quit=0; }
B-3
 II**********************************************************************
II READS THE PALLET AND BOX SET DATA ENTERED BY THE USER FROM // THE INPUT FILE
II**********************************************************************
void inputboxlist(void){
short int n;
char lbl[5], diml[5], dim2[5], dim3[5], boxn[5], strxx[5], stryy[5], strzz[5];
strcpy(strtemp, filename);
strcat(strtemp,".txt");
if ((ifp=fopen(strtemp,"r"))==NULL) {
printf("Cannot open file %s", strtemp); exit(l);
}
tbn=l;
if (fscanf(ifp,"%s %s %s",strxx, stryy, strzz)==EOF) exit(l); xx=atoi(strxx); yy=atoi(stryy); zz=atoi(strzz);
while (fscanf(ifp,"%s %s %s %s %s",lbl,diml,dim2,dim3,boxn)!=EOF){
boxlist[tbn] .diml =atoi(diml);
boxlist[tbn] .dim2=atoi(dim2);
boxlist[tbn] .dim3=atoi(dim3);
boxlistftbn]. vol=boxlist[tbn] .diml *boxlist[tbn] .dim2*boxlist[tbn] .dim3; n=atoi(boxn); boxlist[tbn].n=n;
while (~n) boxlist[tbn+n]=boxlist[tbn]; tbn=tbn+atoi(boxn);
} -tbn;
fclose(ifp); return;
B-4
 // ITERATIONS ARE DONE AND PARAMETERS OF THE BEST SOLUTION ARE // FOUND
void execiterations(void){
for (variant=l; (variant<=6) & Iquit; variant++){
switch(variant){ case 1:
px=xx; py=yy; pz=zz;
break; case 2:
px=zz; py=yy; pz=xx;
break; case 3:
px=zz; py=xx; pz=yy;
break; case 4:
px=yy; py=xx; pz=zz;
break; case 5:
px=xx; py=zz; pz=yy;
break; case 6:
px=yy; py=zz; pz=xx; break;
} listcanditlayers();
layers[0] .layereval=-1;
qsort(layers,layerlistlen+1 ,sizeof(struct layerlist),complayerlist);
for (layersindex=l; (layersindex<=layerlistlen) & Iquit; layersindex++){
++itenum;
time(&finish);
elapsedtime = difftime( finish, start);
printf("VARIANT: %5d; ITERATION (TOTAL): %5d; BEST SO
FAR: %.3f %%; TIME: %.0f",
variant, itenum, percentageused, elapsedtime); packedvolume=0.0;
packedy=0;
packing=l; layerthickness^layerstlayersindexj.layerdim; itelayer=layersindex;
remainpy=py; remainpz=pz; packednumbox=0;
for (x=l; x<=tbn; x++) boxlist[x].packst=0;
B-5
 do{
layerinlayer=0;
layerdone=0;
if (packlayer()) exit( 1); packedy=packedy+layerthickness; remainpy=py-packedy;
if (layerinlayer & !quit){
prepackedy=packedy; preremainpy=remainpy; remainpy=layerthickness-prelayer; packedy=packedy-layerthickness+prelayer; remainpz^lilz;
layerthickness=layerinlayer; layerdone=0;
if (packlayerO) exit( 1); packedy=prepackedy; remainpy=preremainpy; remainpz=pz;
} findlayer(remainpy);
}
while (packing & !quit);
if ((packedvolumObestvolume) & !quit) { bestvolume=packedvolume;
bestvariant^variant; bestite=itelayer; bestpackednum=packednumbox;
}
if (hundredpercent) break;
percentageused=bestvolume* 100/totalvolume; printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b
\b\b\b\b\b\b\b\b\b\b\b"); printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b
\b\b\b\b\b\b\b");
}
if (hundredpercent) break;
if ((xx=yy) & (yy==zz)) variant=6;
B-6
 // LISTS ALL POSSIBLE LAYER HEIGHTS BY GIVING A WEIGHT VALUE TO // EACH OF THEM.
void listcanditlayers(void){
char same;
short int exdim,dimdif,dinien2,dimen3,y,z,k; long int layereval;
layerlistlen=0;
for (x=l; x<=tbn; x++){
for(y=l;y<=3;y++){ switch(y) {
case 1:
exdim=boxlist[x] .diml;
dimen2=boxlist[x] .dim2; dimen3=boxlist[x] .dim3; break;
case 2:
exdim=boxlist[x] .dim2;
dimen2=boxlist[x].diml; dimen3=boxlist[x] .dim3; break;
case 3:
exdim^boxlistfx] .dim3;
dimen2=boxlist[x] .dim 1; dimen3=boxlist[x] .dim2; break;
}
if ((exdim>py) || (((dimen2>px) || (dimen3>pz)) &
((dimen3>px) || (dimen2>pz)))) continue;
same=0;
for (k=l; k<=layerlistlen; k++) if (exdim=layers[k].layerdim){
same=l; continue;
}
if (same) continue;
layereval=0;
B-7
 } }
return;
}
for (z=l; z<=tbn; z++){ if(!(x==z)){
dimdif=abs(exdim-boxlist[z].diml);
if (abs(exdim-boxlist[z] .dim2)<dimdif)
dimdif=abs(exdim-boxlist[z].dim2); if (abs(exdim-boxlist[z] .dim3)<dimdif)
dimdif=abs(exdim-boxlist[z].dim3); layereval=layereval+dimdif;
}
} layers[++layerlistlen].layereval=layereval;
layers [layerlistlen].layerdim=exdim;
// REQUIRED FUNCTION FOR QSORT FUNCTION TO WORK
int complayerlist(const void *i, const void *j){ return *( long int*)i-*( long int*)j;
}
// PACKS THE BOXES FOUND AND ARRANGES ALL VARIABLES AND // . RECORDS PROPERLY
int packlayer( void){
short int lenx, lenz, lpz;
if (layerthickness) { packing=0; return 0;
} (*scrapfirst).cumx=px;(*scrapfirst).cumz=0;
for(;!quit;){
if (kbhitO) if (toupper(getch())=='Q') {
quit=l;
printf("\n\nWait please...\n");
} findsmallestz();
B-8
 //*** SITUATION-1: NO BOXES ON THE RIGHT AND LEFT SIDES ***
if (!(*smallestz).pre & !(*smallestz).pos){ lenx=(* smallestz). cumx;
lpz=remainpz-(*smallestz).cumz;
findbox(lenx, layerthickness, remainpy, lpz, lpz); checkfound();
if (layerdone) break;
if (evened) continue; boxlist[cboxi].cox=0;boxlist[cboxi].coy=packedy; boxlist[cboxi].coz=(*smallestz).cumz;
if (cboxx==(*smallestz).cumx)
(*smallestz).cumz=(*smallestz).cumz+cboxz; else {
(*smallestz).pos=malloc(sizeof(structscrappad)); if ((*smallestz).pos==NULL) {
printf("Insufficient memory available\n"); return 1;
} (*((*smallestz).pos)).pos=NULL;
(*((*smallestz).pos)).pre=smallestz; (*((*smallestz).pos)).cumx=(*smallestz).cumx; (*((*smallestz).pos)).cumz=(*smallestz).cumz; (*smallestz).cumx=cboxx; (*smallestz).cumz=(*smallestz).cumz+cboxz;
} volumecheck();
}
//*** SITUATION-2: NO BOXES ON THE LEFT SIDE ***
else if (!(*smallestz).pre) { lenx=(*smallestz).cumx;
lenz=(*((*smallestz).pos)).cumz-(*smallestz).cumz; lpz=remainpz-(* smallestz) .cumz;
findbox(lenx, layerthickness, remainpy, lenz, lpz); checkfound();
if (layerdone) break;
if (evened) continue;
boxlist[cboxi]. coy=packedy; boxlist[cboxi].coz=(*smallestz).cumz; if (cboxx==(*smallestz).cumx) {
boxlist[cboxi].cox=0;
B-9
 if (*smallestz).cumz+cboxz=(*((*smallestz).pos)).cumz) { (*smallestz).cumz=(*((*smallestz).pos)).cumz; (*smallestz).cumx=(*((*smallestz).pos)).cumx; trash=(*smallestz).pos; (*smallestz).pos=(*((*smallestz).pos)).pos;
if ((*smallestz).pos) (*((*smallestz).pos)).pre=smallestz;
free(trash);
}
else (*smallestz).cumz=(*smallestz).cumz+cboxz;
}
else {
boxlist[cboxi].cox=(*smallestz).cumx-cboxx; if((*smallestz).cumz+cboxz==(*((*smallestz).pos)).cumz)
(*smallestz).cumx=(*smallestz).cumx-cboxx; else {
}
} volumecheckQ;
(*((*smallestz).pos)).pre= malloc(sizeof(struct scrappad));
if ((*((*smallestz).pos)).pre=NULL) { printf("Insufficient memory available\n"); return 1;
} (*((*((*smallestz).pos)).pre)).pos=(*smallestz).pos;
(*((*((*smallestz).pos)).pre)).pre=smallestz; (*smallestz).pos=(*((*smallestz).pos)).pre; (*((*smallestz).pos)).cumx=(*smallestz).cumx; (*smallestz).cumx=(*smallestz).cumx-cboxx; (*((*smallestz).pos)).cumz=
(*smallestz).cumz+cboxz;
B-10
 //*** SITUATION-3: NO BOXES ON THE RIGHT SIDE ***
else if (!(*smallestz).pos){ lenx=(*smallestz)xumx-(*((*smallestz).pre)).cumx; lenz=(*((*smallestz).pre)).cumz-(*smallestz).cuinz; lpz=remainpz-(* smallestz). cumz;
findbox(lenx, layerthickness, remainpy, lenz, lpz); checkfound();
if (layerdone) break;
if (evened) continue;
boxlist[cboxi] .coy=packedy; boxlist[cboxi].coz=(*smallestz).cumz; boxlist[cboxi].cox=(*((*smallestz).pre)).cumx;
if (cboxx=(*smallestz).cumx-(*((*smallestz).pre)).cumx) {
if((*smallestz).cumz+cboxz==(*((*smallestz).pre)).cumz){ (*((*smallestz).pre)).cumx=(*smallestz).cumx; (*((*smallestz).pre)).pos=NULL;
free(smallestz);
} else(*smallestz).cumz=(*smallestz).cumz+cboxz;
}
else {
if((*smallestz).cumz+cboxz=(*((*smallestz).pre)).cumz) (*((*smallestz).pre)).cumx=
(*((*smallestz).pre)).cumx+cboxx;
else {
(*((*smallestz).pre)).pos=
}
} volumecheck();
malloc(sizeof(struct scrappad)); if ((*((*smallestz).pre)).pos=NULL) {
printf("Insufficient memory available"); return 1;
} (*((*((*smallestz).pre)).pos)).pre=(*smallestz).pre;
(*((*((*smallestz).pre)).pos)).pos=smallestz; (*smallestz).pre=(*((*smallestz).pre)).pos; (*((*smallestz).pre)).cumx=
(*((*((*smallestz).pre)).pre)).cumx+cboxx; (*((*smallestz).pre)).cumz=
(*smallestz).cumz+cboxz;
B-ll
 //*** SITUATION-4: THERE ARE BOXES ON BOTH OF THE SIDES *** //*** SUBSITUATION-4A: SIDES ARE EQUAL TO EACH OTHER ***
else if ((*((*smallestz).pre)).cumz==(*((*smallestz).pos)).cumz) { lenx=(*smallestz).cumx-(*((*smallestz).pre)).cumx; lenz=(*((*smallestz).pre))xumz-(*smallestz).cumz; lpz=remainpz-(*smallestz).cumz;
findbox(lenx, layerthickness, remainpy, lenz, lpz); checkfound();
if (layerdone) break;
if (evened) continue;
boxlist[cboxi] .coy=packedy;boxlist[cboxi] .coz=(*smallestz).cumz; if (cboxx==(*smallestz).cumx-(*((*smallestz).pre)).cumx) {
boxlist[cboxi].cox=(*((*smallestz).pre)).cumx; if((*smallestz).cumz+cboxz=(*((*smallestz).pos)).cumz){
(*((*smallestz).pre)).cumx= (*((*smallestz).pos)).cumx;
if ((*((*smallestz).pos)).pos) { (*((*smallestz).pre)).pos=
(*((*smallestz).pos)).pos; (*((*((*smallestz).pos)).pos)).pre=
free(smallestz);
}
else {
}
(*((*smallestz).pre)).pos=NULL; free(smallestz);
} else(*smallestz).cumz=(*smallestz).cumz+cboxz;
}
else if ((*((*smallestz).pre)).cumx<px-(*smallestz).cumx) {
if((*smallestz).cumz+cboxz=(*((*smallestz).pre)).cumz){ (*smallestz).cumx=(*smallestz).cumx-cboxx; boxlist[cboxi].cox=(*smallestz).cumx-cboxx;
}
else {
boxlist[cboxi].cox=(*((*smallestz).pre)).cumx; (*((*smallestz).pre)).pos=
malloc(sizeof(struct scrappad)); if ((*((*smallestz).pre)).pos==NULL) {
printf("Insufficient memory available\n"); return 1;
} (*((*((*smallestz).pre)).pos)).pre=(*smallestz).pre;
B-12
(*smallestz).pre;
 }
}
else {
}
else {
}
(*((*((*smallestz).pre)).pos)).pos=smallestz; (*smallestz).pre=(*((*smallestz).pre)).pos; (*((*smallestz).pre)).cumx=
(*((*((*smallestz).pre)).pre)).cumx+cboxx; (*((*smallestz).pre)).cumz=
(*smallestz).cumz+cboxz;
if((*smallestz).cumz+cboxz==(*((*smallestz).pre)).cumz){ (*((*smallestz).pre)).cumx=
(*((*smallestz).pre)).cumx+cboxx; boxlist[cboxi].cox=(*((*smallestz).pre)).cumx;
boxlist[cboxi].cox=(*smallestz).cumx-cboxx; (*((*smallestz).pos)).pre=
malloc(sizeof(struct scrappad)); if ((*((*smallestz).pos)).pre=NULL) {
printf("Insufficient memory available\n"); return 1;
} (*((*((*smallestz).pos)).pre)).pos=(*smallestz).pos;
(*((*((*smallestz).pos)).pre)).pre=smallestz; (*smallestz).pos=(*((*smallestz).pos)).pre; (*((*smallestz).pos)).cumx=(*smallestz).cumx; (*((*smallestz).pos)).cumz=
(*smallestz).cumz+cboxz; (*smallestz).cumx=(*smallestz).cumx-cboxx;
} volumecheckQ;
}
//*** SUBSITUATION-4B: SIDES ARE NOT EQUAL TO EACH OTHER ***
else {
lenx=(*smallestz).cumx-(*((*smallestz).pre)).cumx; lenz=(*((*smallestz).pre)).cumz-(*smallestz).cumz; lpz=remainpz-(* smallestz) .cumz;
findbox(lenx, layerthickness, remainpy, lenz, lpz);
checkfound();
if (layerdone) break;
if (evened) continue;
boxlist[cboxi] .coy=packedy;boxlist[cboxi] .coz=(*smallestz).cumz;
B-13
 }
}
return 0;
boxlist[cboxi].cox=(*((*smallestz).pre)).cumx;
if (cboxx==(*smallestz).cumx-(*((*smallestz).pre)).cumx) {
if((*smallestz).cumz+cboxz==(*((*smallestz).pre)).cnmz) { (*((*smallestz).pre)).cumx=(*smallestz).cumx; (*((*smallestz).pre)).pos=(*smallestz).pos; (*((*smallestz).pos)).pre=(*smallestz).pre; free(smallestz);
} else(*smallestz).cumz=(*smallestz).cumz+cboxz;
}
else {
} else{
if((*smallestz).cumz+cboxz==(*((*smallestz).pre)).cumz) (*((*smallestz).pre)).cumx=
(*((*smallestz).pre)).cumx+cboxx; else if ((*smallestz).cumz+cboxz==
(*((*smallestz).pos)).cumz) { boxlist[cboxi].cox=(*smallestz).cumx-cboxx; (*smallestz).cumx=(*smallestz).cumx-cboxx;
}
} volumecheck();
(* ((* smallestz).pre)) .pos= malloc(sizeof(struct scrappad));
if ((*((*smallestz).pre)).pos=NULL) { printf("Insufficient memory available\n"); return 1;
} (*((*((*smallestz).pre)).pos)).pre=(*smallestz).pre;
(*((*((*smallestz).pre)).pos)).pos=smallestz; (*smallestz).pre=(*((*smallestz).pre)).pos; (*((*smallestz).pre)).cumx=
(*((*((*smallestz).pre)).pre)).cumx+cboxx;
(*((*smallestz).pre)).cumz= (*smallestz).cumz+cboxz;
B-14
 //**********************************************************************
// FINDS THE MOST PROPER LAYER HIGHT BY LOOKING AT THE UNPACKED // BOXES AND THE REMAINING EMPTY SPACE AVAILABLE
//**********************************************************************
int findlayer( short int thickness) {
  short int exdim,dimdif,dimen2,dimen3,y,z; long int layereval, eval;
  layerthickness=0;
  eval= 1000000;
  for(x=1;x<=tbn;x++){
    if (boxlist[x].packst) continue;
    for(y=1;y<=3;y++){
      switch(y) {
      case 1:
        exdim=boxlist[x].diml;
        dimen2=boxlist[x].dim2;
        dimen3=boxlist[x] .dim3;
        break;
      case 2:
        exdim=boxlist[x].dim2;
        dimen2=boxlist[x].dim1;
        dimen3=boxlist[x].dim3;
        break;
      case 3:
        exdim=boxlist[x].dim3;
        dimen2=boxlist[x].diml;
        dimen3=boxlist[x].dim2;
        break;
      }
      layereval=0;
      if ((exdim<=thickness) & (((dimen2<=px) &
          (dimen3<=pz)) || ((dimen3<=px) & (dimen2<=pz)))) {
        for(z=1;z<=tbn;z++){
          if (!(x==z) & !(boxlist[z].packst)){
            dimdif=abs(exdim-boxlist[z].diml);
            if (abs(exdim-boxlist[z].dim2)<dimdif)
              dimdif=abs(exdim-boxlist[z].dim2);
            if (abs(exdim-boxlist[z].dim3)<dimdif)
              dimdif=abs(exdim-boxlist[z].dim3);
            layereval=layereval+dimdif;
          }
        }
        if (layereval < eval) {
          eval=layereval;
          layerthickness=exdim;
        }
      }
    }
  }
  if (layerthickness==0 || layerthickness>remainpy) {
    packing=0;
    return 0;
  }
}
//FINDSTHEMOSTPROPERBOXESBYLOOKINGATALLSEXPOSSIBLE // ORIENTATIONS, EMPTY SPACE GIVEN, ADJACENT BOXES,
// AND PALLET LIMITS
void findbox(short int hmx, short int hy, short int hmy, short int hz, short int hmz){ short int y;
bfx=32767; bfy=32767; bfz=32767; bbfx=32767; bbfy=32767; bbfz=32767; boxi=0; bboxi=0;
for (y=l; y<=tbn; y=y+boxlist[y].n){
} }
for (x=y; x<x+boxlist[y].n-l; x++) if (!boxlist[x].packst) break;
if (boxlist[x].packst) continue;
if (x>tbn) return;
analyzebox (hmx, hy, hmy, hz, hmz, boxlist[x].diml, boxlist[x].dim2,
boxlist[x].dim3);
if ((boxlist[x].diml=boxlist[x].dim3) & (boxlist[x].dim3==boxlist[x].dim2)) continue;
analyzebox (hmx, hy, hmy, hz, hmz, boxlist[x].diml, boxlist[x].dim3, boxlist[x].dim2);
analyzebox (hmx, hy, hmy, hz, hmz, boxlist[x].dim2, boxlist[x].diml, boxlist[x].dim3);
analyzebox (hmx, hy, hmy, hz, hmz, boxlist[x].dim2, boxlist[x].dim3, boxlist[x].diml);
analyzebox (hmx, hy, hmy, hz, hmz, boxlist[x].dim3, boxlist[x].diml, boxlist[x].dim2);
analyzebox (hmx, hy, hmy, hz, hmz, boxlist[x].dim3, boxlist[x].dim2, boxlist[x].diml);
B-16
 // ANALYZES EACH UNPACKED BOX TO FIND THE BEST FITTING ONE TO // THE EMPTY SPACE GIVEN
//J»-'--'--'--'--' *-'--'--'--'--' '-' *-'»-t»-'»-' ***--** '-*'-^»>i^>t«A >tjA A A A A >>.^.A A .1..t.A A A A A A A A »l*»l*»l»»U»t»«I«»1»«I»«t»«IJ«I»»U«1»»i««1*«1»*l»*1»«I»«1««1»«1» !» !  //T*^^Vn*VV^VVV^T*^V^V^T*V^^^^V^^^^^^^^^^V^*r^"P^**V*V^^^T**^*********^^*^^^ •
void analyzebox (short int hmx, short int hy, short int hmy, shortinthz, shortinthmz,
short int dim1, short int dim2, short int dim3){ if (diml<=hmx && dim2<=hmy && dim3<=hmz){
if (dim2<=hy) {
if (hy-dim2<bfy) {
boxx=diml; boxy=dim2; boxz=dim3;
bfx=hmx-diml; bfy=hy-dim2; bfz=abs(hz-dim3); boxi=x;
}
else if (hy-dim2==bfy && hmx-diml<bfx) {
boxx=diml; boxy=dim2; boxz=dim3;
bfx=hmx-diml; bfy=hy-dim2; bfz=abs(hz-dim3); boxi=x;
}
else if (hy-dim2==bfy && hmx-diml=
}
}
else {
bfx && abs(hz-dim3)<bfz) { boxx=diml; boxy=dim2; boxz=dim3;
bfx=hmx-diml; bfy=hy-dim2; bfz=abs(hz-dim3); boxi=x;
if (dim2-hy<bbfy) {
bboxx=diml; bboxy=dim2; bboxz=dim3; bbfx=hmx-diml; bbfy=dim2-hy; bbfz=abs(hz-dim3); bboxi=x;
}
else if (dim2-hy==bbfy && hmx-diml<bbfx) {
bboxx=diml; bboxy=dim2; bboxz=dim3; bbfx=hmx-diml; bbfy=dim2-hy; bbfz=abs(hz-dini3); bboxi=x;
}
else if (dim2-hy==bbfy &&
hmx-diml=bbfk && abs(hz-dim3)<bbfz) { bboxx=diml; bboxy=dim2; bboxz=dim3;
bbfx=hmx-diml; bbfy=dim2-hy; bbfz=abs(hz-dim3); bboxi=x;
}
// FINDS THE FIRST TO BE PACKED GAP IN THE LAYER EDGE
//********************************************************
void findsmallestz(void) { scrapmemb=scrapfirst; smallestz=scrapmemb;
while (!((*scrapmemb).pos=NULL)) { if((*((*scrapmemb).pos)).cumz<(*smallestz).cumz)
scrapmemb=(*scrapmemb).pos;
} return;
} //************************************************************
// AFTER FINDING EACH BOX, THE CANDIDATE BOXES AND THE // CONDITION OF THE LAYER ARE EXAMINED
//************************************************************
void checkfound(void){ evened=0;
if (boxi) { cboxi=boxi;cboxx=boxx;cboxy=boxy;cboxz=boxz;
}
else {
}
else {
if ((bboxi>0) & (layerinlayer || (!(*smallestz).pre & !(*smallestz).pos))){ if (Ilayerinlayer) {
prelayer=layerthickness; lilz=(* smallestz). cumz;
}: cboxi=bboxi;cboxx=bboxx;cboxy=bboxy;cboxz =bboxz;
layerinlayer=layerinlayer+bboxy-layerthickness; layerthickness=bboxy;
if (!(*smallestz).pre & !(*smallestz).pos) layerdone=l; else {
evened=l;
if (!(*smallestz).pre) {
trash=(*smallestz).pos; (*smallestz).cumx=(*((*smallestz).pos)).cumx; (*smallestz).cumz=(*((*smallestz).pos)).cumz; (*smallestz).pos=(*((*smallestz).pos)).pos;
if ((*smallestz).pos)
B-18
smallestz=(*scrapmemb).pos;
 } }
return; }
free(trash);
}
else {
B-19
(*((*smallestz).pos)).pre=smallestz;
}
else if (!(*smallestz).pos){
(*((*smallestz).pre)).pos=NULL; (*((*smallestz).pre)).cumx=(*smallestz).cumx; free(smallestz);
}
else {
if((*((*smallestz).pre)).cumz== (*((*smallestz).pos)).cumz) {
(*((*smallestz).pre)).pos= (*((*smallestz).pos)).pos;
if ((*((*smallestz).pos)).pos) (*((*((*smallestz).pos)).pos)).pre=
(*smallestz).pre; (*((*smallestz).pre)).cumx=
(*((*smallestz).pos)).cumx; free((*smallestz).pos);
free(smallestz);
(*((*smallestz).pre)).pos=(*smallestz).pos; (*((*smallestz).pos)).pre=(*smallestz).pre; if ((*((*smallestz).pre)).cumz<
(*((*smallestz).pos)).cumz) (*((*smallestz).pre)).cumx=
free(smallestz);
(*smallestz).c\anx;
 // AFTER PACKING OF EACH BOX, 100% PACKING CONDITION IS CHECKED
void volumecheck (void) { boxlist[cboxi].packst=l;
boxlist[cboxi] .packx=cboxx;boxlist[cboxi] .packy=cboxy; boxlist[cboxi] .packz=cboxz; packedvolume=packedvolume+boxlist[cboxi].vol; packednumbox++;
if (packingbest) { graphunpackedout();
outputboxlist();
}
else if (packedvolume==totalvolume || packedvolume=totalboxvol) {
packing=0; hundredpercent=l;
} return;
// DATA FOR THE VISUALIZATION PROGRAM IS WRITTEN TO THE // "VISUDAT" FILE AND THE LIST OF UNPACKED BOXES IS
// MERGED TO THE END OF THE REPORT FILE
void graphunpackedout(void) { charn[5];
if (!unpacked)!
itoa( boxlist[cboxi].cox, strcox, 10); itoa( boxlist[cboxi].coy, strcoy, 10); itoa( boxlist[cboxi].coz, strcoz, 10);
itoa( boxlist[cboxi].packx, strpackx, 10);
itoa( boxlist[cboxi].packy, strpacky, 10);
itoa( boxlist[cboxi].packz, strpackz, 10);
}
else {
}
if (lunpacked) fprintf(gfp,"%5s%5s%5s%5s%5s%5s\n",
strcox,strcoy,strcoz,strpackx,strpacky,strpackz); elsefprintf(ofp,"%5s%5s%5s%5s\n",n,strpackx,strpacky,strpackz);
}
itoa( cboxi, n, 10); itoa( boxlist[cboxi].diml, strpackx, 10); itoa( boxlist[cboxi].dim2, strpacky, 10);
itoa( boxlist[cboxi].dim3, strpackz, 10);
B-20
 //*********************************************************************
// TRANSFORMS THE FOUND COORDINATE SYSTEM TO THE ONE ENTERED // BY THE USER AND WRITES THEM TO THE REPORT FILE
//*********************************************************************
void outputboxlist(void){
char strx[5], strpackst[5], strdiml[5], strdim2[5], strdim3[5], strcox[5], strcoy[5],
strcoz[5], strpackx[5], strpacky[5], strpackz[5]; short int x, y, z, bx, by, bz;
switch(bestvariant) { case 1:
x=boxlist[cboxi].cox; y=boxlist[cboxi].coy; z=boxlist[cboxi].coz; bx=boxlist[cboxi] .packx; by=boxlist[cboxi] .packy; bz=boxlist[cboxi] .packz;
break;
case 2:
x=boxlist[cboxi].coz; y=boxlist[cboxi].coy; z=boxlist[cboxi].cox; bx=boxlist[cboxi] .packz; by=boxlist[cboxi] .packy; bz=boxlist[cboxi] .packx;
break;
case 3:
x=boxlist[cboxi].coy; y=boxlist[cboxi].coz; z=boxlist[cboxi].cox; bx=boxlist[cboxi].packy; by=boxlist[cboxi].packz; bz=boxlist[cboxi] .packx;
break;
case 4:
x=boxlist[cboxi].coy; y=boxlist[cboxi].cox; z=boxlist[cboxi].coz; bx=boxlist[cboxi].packy; by=boxlist[cboxi].packx; bz=boxlist[cboxi] .packz;
break;
case 5:
x=boxlist[cboxi].cox; y=boxlist[cboxi].coz; z=boxlist[cboxi].coy; bx=boxlist[cboxi] .packx; by=boxlist[cboxi] .packz; bz=boxlist[cboxi] .packy;
break;
case 6:
x=boxlist[cboxi].coz; y=boxlist[cboxi].cox; z=boxlist[cboxi].coy; bx=boxlist[cboxi].packz; by=boxlist[cboxi].packx; bz=boxlist[cboxi] .packy;
break;
}
itoa( cboxi, strx,10);
itoa( boxlist[cboxi].packst, strpackst, 10); itoa(boxlist[cboxi].diml, strdiml, 10); itoa( boxlist[cboxi].dim2, strdim2,10) itoa( boxlist[cboxi].dim3, strdim3,10)
B-21
 itoa(x, strcox, 10);
itoa(y, strcoy, 10);
itoa(z, strcoz, 10);
itoa(bx, strpackx, 10);
itoa(by, strpacky, 10);
itoa(bz, strpackz, 10);
boxlist[cboxi].cox=x; boxlist[cboxi].coy=y; boxlist[cboxi].coz=z; boxlist[cboxi] .packx=bx; boxlist[cboxi] .packy=by; boxlist[cboxi] .packz=bz; Q)rintf(oQ),"%5s%5s%9s%9s%9s%9s%9s%9s%9s%9s%9s\n",strx,strpackst,
strdiml,strdim2,strdim3,strcox,strcoy,strcoz,strpackx,strpacky,strpackz); return;
//*******************************************************************
// USING THE PARAMETERS FOUND, PACKS THE BEST SOLUTION FOUND // AND REPORS TO THE CONSOLE AND TO A TEXT FILE
//*******************************************************************
void report(void){ quit=0;
switch(bestvariant) { case 1:
px=xx; py=yy; pz=zz;
break; case 2:
px=zz; py=yy; pz=xx;
break; case 3:
px=zz; py=xx; pz=yy;
break; case 4:
px=yy; py=xx; pz=zz;
break; case 5:
px=xx; py=zz; pz=yy;
break; case 6:
px=yy; py=zz; pz=xx; break;
}
packingbest=l;
if ((gfp=fopen(graphout,"w"))=NULL) {
}
printf("Cannot open file %s", filename); exit(l);
itoa(px, strpx, 10);
itoa(py, strpy, 10);
itoa(pz, strpz, 10); §)rintf(gQ),"%5s%5s%5s\n",strpx,strpy,strpz); strcat(filename,".out");
if ((ofp=fopen(filename,"w"))=NULL) { printf("Cannot open file %s", filename); exit(l);
}
percentagepackedbox=bestvolume* 100/totalboxvol;
percentageused=bestvolume* 100/totalvolume;
elapsedtime = difftime(finish, start);
fprintf(ofp,"       ***REPORT***\n\n");
fprintf(ofp," ");
fprintf(ofp," fprintf(ofp,");
fprintf(ofp," fprintf(ofp,"
fprintf(ofp," fprintf(ofp," fprintf(ofp," fprintf(ofp," fprintf(ofp," fprintf(ofp," fprintf(ofp,"
= difftime( finish, start);
*** REPORT ***\n\n"); ELAPSED TIME :
Almost %.0f sec\n", elapsedtime); TOTAL NUMBER OF ITERATIONS DONE : %d\n", itenum);
BEST SOLUTION FOUND AT : ITERATION: %d OF VARIANT: %d\n", bestite, bestvariant);
TOTAL NUMBER OF BOXES PACKED NUMBER OF BOXES
: %d\n", tbn); :
%d\n", bestpackednum);
TOTAL VOLUME OF ALL BOXES
PALLET VOLUME
BEST SOLUTION'S VOLUME UTILIZATION
: %.0f\n", totalboxvol);
: %.0f\n", totalvolume);
: %.0f OUT OF %.0f\n", bestvolume, totalvolume);
PERCENTAGE OF PALLET VOLUME USED : %.6f %%\n", percentageused);
PERCENTAGE OF PACKED BOXES (VOLUME) : %.6f %%\n", percentagepackedbox);
WHILE PALLET ORIENTATION :
X=%d; Y=%d; Z= %d\n", px, py, pz);
\n");
NO: PACKSTA DIMEN-1 DMEN-2 DIMEN-3 COOR-X COOR-Y COOR-Z PACKEDX PACKEDY PACKEDZ\n);
listcanditlayers();
layers[0] .layereval=-1; qsort(layers,layerlistlen+l,sizeof(structlayerlist),complayerlist); packedvolume=0.0;
packedy=0;
packing=l;
B-23
\n»);
 layerthickness=layers[bestite].layerdim; remainpy=py; remainpz=pz;
for (x=l; x<=tbn; x++) boxlist[x].packst=0; do{
  layerinlayer^O;
  layerdone=0;
  packlayer();
  packedy=packedy+layerthickness;
  remainpy=py-packedy;
  if(layerinlayer){
    prepackedy=packedy; preremainpy=remainpy; remainpy=layerthickness-prelayer; packedy=packedy-layerthickness+prelayer; remainpz=lilz;
    layerthickness=layerinlayer; layerdone=0;
    packlayer(); packedy=prepackedy; remainpy=preremainpy; remainpz=pz;
  }
  if (!quit)
    findlayer(remainpy);
}
while (packing & !quit);
fprintf(ofp,"\n\n *** LIST OF UNPACKED BOXES ***\n"); unpacked=1;
for (cboxi=l; cboxi<=tbn; cboxi++) if (!boxlist[cboxi].packst)
graphunpackedoutO;
unpacked=0; fclose(ofp); fclose(gfp); printf("\n");
for (n=l; n<=tbn; n++)
if (boxlist[n].packst) printf("%d %d %d %d %d %d %d %d %d %d\n",n, boxlist[n].diml,boxlist[n].dim2,boxlist[n].dim3,boxlist[n].cox, boxlist[n] .coy,boxlist[n] .coz,boxlist[n] .packx,boxlist[n].
packy,boxlist[n] .packz); printf(" ELAPSED TIME : Almost %.0f sec\n",
elapsedtime); printf(" TOTAL NUMBER OF ITERATIONS DONE : %d\n", itenum);
printfC BEST SOLUTION FOUND AT :
ITERATION: %d OF VARIANT: %d\n", bestite, bestvariant);
printf(" TOTAL NUMBER OF BOXES : %d\n", tbn); printf(" PACKED NUMBER OF BOXES : %d\n",
bestpackednum);
B-24
 printf(" TOTAL VOLUME OF ALL BOXES : %.0f\n", totalboxvol);
printf("PALLETVOLUME :%.0f\n",totalvolume); printf("BESTSOLUTION'SVOLUMEUTILIZATION :%.0fOUTOF
%.0f\n", bestvolume, totalvolume); printfC PERCENTAGE OF PALLET VOLUME USED : %.6f %%\n",
percentageused); printf("PERCENTAGEOFPACKEDBOXES(VOLUME) :%.6f%%\n",
percentagepackedbox); printf(" WHILE PALLET ORIENTATION :
X=%d; Y=%d; Z= %d\n\n\n", px, py, pz); printf(" TO VISUALIZE THIS SOLUTION, PLEASE RUN 'VISUAL.EXEV);
B-25
 Appendix C - The C Program Code of the Visualizer
// INCLUDED HEADER FILES
#include <stdio.h> #include <stdlib.h> #include <stdarg.h> #include <graphics.h>
/* Function prototypes */
void Initialize(void); void Pack(void);
void PutBox(void);
void SayGoodbye(void); void Pause(void);
void MainWindow(char *header);
void StatusLine(char *msg);
void DrawBorder(void);
void changetextstyle(int font, int direction, int charsize); int gprintf(int *xloc, int *yloc, char *fint,. ..) ;
// VARIABLE, CONSTANT AND STRUCTURE DECLARATIONS
int GraphDriver; int GraphMode; double AspectRatio; int MaxX, MaxY; int MaxColors;
strpacky[8], strpackz[8]; FILE *igf;
/* The Graphics device driver */
/* The Graphics mode value */
/* Aspect ratio of a pixel on the screen*/
/* The maximum resolution of the screen */
/* The maximum # of colors available */ /* Reports any graphics errors */
int ErrorCode;
struct palettetype palette;
struct dataarray {int ex, cy, cz, pax, pay, paz; } data[2000];
int px, py, pz, cox, coy, coz, packx, packy, packz, a, b, index, currenty, q;
double max, sc;
char strpx[8], strpy[8], strpz[8], oldstrcoy[8], strcox[8], strcoy[8], strcoz[8], strpackx[8],
/* Used to read palette info */
C-l
 /* BEGIN MAIN FUNCTION */
int main()
{ Initialize();
Pack(); SayGoodbye(); closegraph(); return(O);
}
/* Set system into Graphics mode */
/* Give user the closing screen */
/*Returnthesystemtotextmode */
/* INITIALIZE: INITIALIZES THE GRAPHICS SYSTEM AND */ /* REPORTS ANY ERRORS WHICH OCCURED. */
void Initialize(void)
{
int xasp, yasp; /* Used to read the aspect ratio*/
GraphDriver = DETECT; /* Request auto-detection */ initgraph( &GraphDriver, &GraphMode,"");
ErrorCode = graphresult(); /* Read result of initialization*/ if( ErrorCode != grOk) { /* Error occured during init */
printf(" Graphics System Error: %s\n", grapherrormsg( ErrorCode));
exit( 1);
Y
getpalette( &palette);
MaxColors = getmaxcolor() + 1;
MaxX = getmaxx();
MaxY = getmaxyO;
getaspectratio( &xasp, &yasp);
AspectRatio = (double)xasp / (double)yasp; /* Get correction factor */
}
/* Read the palette from board */ /* Read maximum number of colors*/
/* Read size of screen */ /* read the hardware aspect */
C-2
 /* PACK: READS THE DATA FROM "VISUDAT" FILE AND ARRANGES */ /* BOXES TO PACK FROM FAR END TO THE CLOSE */
void Pack(void)
{
struct viewporttype vp;
charbuffer[10];
MainWindow( "PACKING OF THE BEST SOLUTION FOUND" ); if ((igf=fopen("visudat","r"))=NULL) {
outtextxy(0,0, "CANNOT OPEN FILE visudat"); exit(l);
}
fscanf(igf,"%s %s %s",strpx, strpy, strpz);
px=atoi(strpx); py=atoi(strpy); pz=atoi(strpz); max=px;
if (py>max) max=py;
if (pz>max) max=pz;
sc=120/max;
getviewsettings( &vp);
settextjustify( CENTER_TEXT, TOP_TEXT );
changetextstyle( TRIPLEX_FONT, HORIZ_DIR, 4);
changetextstyle( DEFAULT_FONT, HORIZ_DIR, 1);
setviewport( vp.left+50, vp.top+40, vp.right-50, vp.bottom-10,1);
getviewsettings( &vp );
settextjustify( CENTER_TEXT, CENTER_TEXT);
outtextxy( 20, 0, "PRESS 'Q' TO QUIT" );
setcolor(3);
outtextxy( 220, 0, "PALLET ORIENTATION (X Y Z) : " );
outtextxy( 350, 0, strpx);
outtextxy( 390, 0, strpy);
outtextxy( 430, 0, strpz);
setfillstyle( EMPTY_FILL, 1);
bar3d( 10, 350-sc*2*py, 10+sc*2*px, 350, sc*pz, 1);
outtextxy(460, 30, "COORDINATES:");
outtextxy(460,40, "CX: CY: CZ:");
outtextxy(460, 70, "DIMENSIONS:");
outtextxy(460,80, "DX: DY: DZ:");
index=l;
fscanf (igf,"%s %s %s %s %s %s", strcox, strcoy, strcoz, strpackx, strpacky, strpackz); cox=atoi(strcox); coy=atoi(strcoy); coz=atoi(strcoz);
packx=atoi(strpackx); packy=atoi(strpacky); packz=atoi(strpackz); data[index].cx=cox; datafindexj.cy^coy; data[index].cz=coz;
data[index].pax=packx; data[index].pay=packy; data[index].paz=packz;
C-3
 index++; currenty=data[index].cy;
while (fscanf (igf,"%s %s %s %s %s %s", strcox, strcoy, strcoz, strpackx, strpacky,
strpackz)!=EOF){
cox=atoi(strcox); coy=atoi(strcoy); coz=atoi(strcoz); packx=atoi(strpackx); packy=atoi(strpacky); packz=atoi(strpackz); data[index].cx=cox; data[index].cy=coy; data[index].cz=coz; data[index].pax=packx; data[index].pay=packy; data[index].paz=packz;
if (data[index].cy!=currenty){ b=index; index--;
PutBox();
} index++;
if (q==l) return;
} index--;
PutBox(); fclose(igf); Pause();
}
/* Pause for user's response */
/**********PUTBOX: DRAW BOXES INTHEIRLOCATIONS **********/
void PutBox(void)
{
for (a=index; a>0; a~){
setcolor(O); ^^^^^^^^^__^^^_^^—1^^^___^^^_1__ 50,"  "); 90,"||");
setcolor(random(15)+l);
itoa(data[a].pax, strpackx, 10); outtextxy(410, 90, strpackx); itoa(data[a].pay, strpacky, 10); outtextxy(460, 90, strpacky); itoa(data[a].paz, strpackz, 10); outtextxy(508, 90, strpackz); itoa(data[a].cx, strcox, 10); outtextxy(410,50, strcox); itoa(data[a].cy, strcoy, 10); outtextxy(460, 50, strcoy); itoa(data[a].cz, strcoz, 10); outtextxy(508, 50, strcoz); bar3d( 10+sc*2*data[a].cx+sc*data[a].cz,
350-sc*2*data[a].cy-sc*.74*data[a].cz-sc*2*data[a].pay, 10+sc*2*data[a].cx+sc*data[a].cz+sc*2*data[a].pax, 350-sc*2*data[a].cy-sc*.74*data[a].cz, sc*data[a].paz, 1);
if (toupper(getch())=,Q'){ q=l; break;}
}
data[1].cx=data[b].ex; data[1].cy=data[b].cy;
data[1].cz=data[b].cz; data[1].pax=data[b].pax; data[1].pay=data[b].pay; data[1].paz=data[b].paz; index=l;
currenty=data[index] .cy;
}
C-4
 /* SAYGOODBYE: GIVE A CLOSING SCREEN */ /* TO THE USER BEFORE LEAVING. */
void SayGoodbye(void)
{ structviewporttypeviewinfo;
/*Structuretoreadviewport */
MainWindow( "== The End ==");
getviewsettings( &viewinfo);
changetextstyle( TRIPLEX_FONT, HORIZJDIR, 4); settextjustify( CENTER_TEXT, CENTERJTEXT );
h = viewinfo.bottom - viewinfo.top;
w=viewinfo.right -viewinfo.left;
outtextxy( w/2, h/2, "That's all, folks!");
StatusLine( "Press any key to EXIT");
getch();
cleardevice(); /* Clear the graphics screen */
}
int h, w;
/* PAUSE: PAUSE UNTIL THE USER ENTERS A KEYSTROKE.
void Pause(void)
{
static char msg[] = "Esc aborts or press a key...";
*/
*/
intc;
StatusLine( msg); c=getch();
if(0 = c){
c = getch(); cleardevice();
/* Put msg at bottom of screen
/* Read a character from kbd */ /* Did use hit a non-ASCII key? */
/* Read scan code for keyboard */
/* Clear the screen */
}
}
/* Read viewport settings */
C-5
 /* MAINWINDOW: ESTABLISH THE MAIN WINDOW */
void MainWindow( char *header)
{
int height;
cleardevice(); /* Clear graphics screen */ setcolor(MaxColors-1); /*Setcurrentcolortowhite */ setviewport( 0, 0, MaxX, MaxY, 1); /* Open port to foil screen */ height = textheight( "H"); /* Get basic text height */ changetextstyle( DEFAULT_FONT, HORIZ.DIR, 1);
settextjustify( CENTER TEXT, TOP_TEXT );
outtextxy( MaxX/2,2, header);
setviewport( 0, height+4, MaxX, MaxY-(height+4), 1);
DrawBorderO;
setviewport( 1, height+5, MaxX-1, MaxY-(height+5), 1);
}
/* STATUSLINE: DISPLAY A STATUS LINE */ /* AT THE BOTTOM OF THE SCREEN. */
void StatusLine( char *msg)
{
int height;
setviewport( 0,0, MaxX, MaxY, 1); /* Open port to foil screen */ setcolor(MaxColors-1); /*Setcurrentcolortowhite */ changetextstyle( DEFAULT.FONT, HORIZ_DIR, 1);
settextjustify( CENTER_TEXT, TOP_TEXT );
setlinestyle( SOLID_LINE, 0, NORM_WIDTH );
setfillstyle( EMPTY_FILL, 0 );
height = textheight( "H"); /* Detemine current height */ bar( 0, MaxY-(height+4), MaxX, MaxY);
rectangle( 0, MaxY-(height+4), MaxX, MaxY);
outtextxy( MaxX/2, MaxY-(height+2), msg);
setviewport( 1, height+5, MaxX-1, MaxY-(height+5), 1);
}
C-6
 /* DRAWBORDER: DRAW A SOLID SINGLE LINE */ /* AROUND THE CURRENT VIEWPORT. */
void DrawBorder(void)
{
struct viewporttype vp;
setcolor(MaxColors-1); /*Setcurrentcolortowhite */ setlinestyle( SOLID_LINE, 0,NORM.WIDTH);
getviewsettings( &vp);
rectangle( 0, 0, vp.right-vp.left, vp.bottom-vp.top);
}
/* CHANGETEXTSTYLE: SIMILAR TO SETTEXTSTYLE, BUT CHECKS FOR */ /* ERRORS THAT MIGHT OCCUR WHIL LOADING THE FONT FILE. */
/###**####**#********#********##******#******«
void changetextstyle(int font, int direction, int charsize)
{
int ErrorCode;
gfaphresultO; /* clear error code settextstyle(font, direction, charsize);
ErrorCode = graphresult(); /* check result
if( ErrorCode != grOk) { /* if error occured
*/
*/ */
closegraph();
printf(" Graphics System Error: %s\n", grapherrormsg( ErrorCode)); exit( 1);
} }
C-7
 /* GPRINTF: USED LIKE PRINTF EXCEPT THE OUTPUT IS SENT TO THE */ /* SCREEN IN GRAPHICS MODE AT THE SPECIFIED CO-ORDINATE. */
/*********************************************************************/
int gprintf( int
