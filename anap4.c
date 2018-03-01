#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <TFile.h>
#include <TTree.h>
#include <TCut.h>
#include <fstream>
#include <TLorentzVector.h>

#define B_SIZE 16384

/* edit here */
#define N_QDC 8
#define N_TDC 5

TFile *file;
TTree *tree;

/** tree variables **/
/* raw */
unsigned int q0[N_QDC];
unsigned int t0[N_TDC];

/* pedestal subtracted */
double q1[N_QDC];

void analysis(char *filename);
void anablk(unsigned short *rawbuf);
int  anaevt(int evtlen, unsigned short *rawbuf);
void error_exit(TFile *file, int i);


int main(int iarg, char *argv[]) {
  if (iarg != 3) {
    printf("usage: ./anap4 <input> <output>\n");
    printf("eg.)\n");
    printf("$ ./anap4 rdf/0123.rdf rt/0123.root\n");
    exit(0);
  }

  /* TFile */
  file = new TFile(argv[2], "recreate");
  tree = new TTree("p4", "p4");
  /** tree variables **/
  /* raw */
  tree->Branch("q0", q0, Form("q0[%d]/i",N_QDC));
  tree->Branch("t0", t0, Form("t0[%d]/i",N_TDC));
  /* pedestal subtracted */
  tree->Branch("q1", q1, Form("q1[%d]/D",N_QDC));
 
  analysis(argv[1]);
 
  /* close histograms */
  tree->Write();
  file->Close();
  return 0;
}

void analysis(char *filename) {
  int fdin, readbyte;
  unsigned short rawbuf[B_SIZE/2];

  /* open data file */
  fdin = open(filename,O_RDONLY,NULL);
  if (fdin == -1) error_exit(file, 0);

  /* Header Block */
  readbyte = read(fdin,rawbuf,B_SIZE);
  if (readbyte<B_SIZE || rawbuf[0] != 0x0001) error_exit(file, 1);

  while (1) {
    readbyte = read(fdin,rawbuf,B_SIZE);
    if (readbyte<B_SIZE) {
      break;
      error_exit(file, 2);
    }
    if (rawbuf[0] == 0x0000) { /* Event Block */
      anablk(rawbuf);
    } else if (rawbuf[0] == 0xffff) { /* Ender Block */
      break;
    } else {
      error_exit(file, 3);
    }
  }
  close(fdin);
}

void anablk(unsigned short *rawbuf) {
  int ievt=0, idevt, ip=4, evtsize, idf, ipnext;

  while (1) {
    evtsize = (rawbuf[ip++]&0x7fff);
    if (evtsize==0x7fff) break;
    ievt++;
    ipnext = ip+evtsize-1;
    idf = rawbuf[ip++];
    idevt = rawbuf[ip++];
    if (ievt!=idevt) error_exit(file, 4);
    /* event analysis routine */
    anaevt(ipnext-ip,rawbuf+ip);
    ip += (ipnext-ip);
  }
}

void error_exit(TFile *file, int i) {
  printf("error exit [%d]\n", i); fflush(stdout);
  file->Close();
  exit(0);
}

int anaevt(int evtlen, unsigned short *rawbuf) {
  int i, ip;
  int segsize, ids, ipsn;


  /* clear event buffer */
  for (i=0; i<N_QDC; i++) q0[i] = 0;
  for (i=0; i<N_TDC; i++) t0[i] = 0;
 
  /* decode event data */
  ip = 0;
  while (ip<evtlen) {
    int tmpdat, tmpch;

    /* segment header */
    segsize = rawbuf[ip++];
    ipsn = ip+segsize-1;
    ids = rawbuf[ip++];

    while (ip<ipsn) { /* segment loop */
      for (i=0; i<N_QDC; i++) {
        tmpdat = rawbuf[ip++];
        tmpch = (tmpdat>>12)&0xf;
        q0[i] = (tmpdat&0xfff);
      }
      for (i=0; i<N_TDC; i++) {
        tmpdat = rawbuf[ip++];
        tmpch = (tmpdat>>12)&0xf;
        t0[i] = (tmpdat&0xfff);
      }
    }
  }
  
  /** calculation **/
  /* pedestal subtraction */
  const double cq0[N_QDC] = {9.108,9.7784,14.658,20.033,3.1289,3.3119,2.7670,18.922};
  for (i=0; i<N_QDC; i++) q1[i] = q0[i] - cq0[i];
   
  tree->Fill();
  return (ip);
}

