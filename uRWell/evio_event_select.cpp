#include <stdio.h>
#include <string.h>
#include <evio.h>
#include <evioBankUtil.h>

#include <iostream>

using namespace std;

#define EV_BUFFER_LEN   10000000

int evFileNumber = 0;
int evInputHandle = 0;
int evOutputHandle = 0;
int evEventNumber = 0;
unsigned int evBuffer[EV_BUFFER_LEN];

// char *file_data_in = "/data/clas_004759";
// char *file_numbers_in = "/data/run_4759_failed.txt";
//char *file_data_in = "../urwell_001164";
//char *file_numbers_in = "/home/rafopar/work/git/clas12AnaTools/uRWell/Data/Skim_EvNumbers_1164.dat";
char *file_numbers_in;
char *ev_numbers_in;

int main(int argc, char *argv[])
{
  
  int run, event_num, sector_num, result;
  double p, theta, phi;
  int status;
  char buf[1200];
  int itmp, ievent, last_event_num;

  char *file_data_in;
  char *file_data_out;

  GET_PUT_INIT;

  if( argc < 2 ){
    cout<<"Please specify the run number "<<endl;
    cout<<"Exiting "<<endl;
    exit(1);
  }


  run = atoi(argv[1]);
  sprintf(file_data_in, "/work/clas12/rafopar/uRWELL/Readout/APV25/urwell_00%d.evio.00000", run);
  sprintf(file_data_out, "Data/SkimCoincidence_%d.evio", run);
  sprintf(ev_numbers_in, "Data/Skim_EvNumbers_%d.dat", run);
  FILE *f = fopen(ev_numbers_in, "rt");


  // evio input file
  //sprintf(buf, "%s.evio.%05d", file_data_in, evFileNumber++);
  if((status = evOpen(file_data_in, "r", &evInputHandle)) != 0)
    printf("\n ?Unable to open input file %s, status=%d\n\n",buf,status);
  else
    printf("\n Successfully opened evio input file %s\n\n", buf);

  // evio output file
  //sprintf(buf, "%s_failed.evio.0", file_data_in);
  if((status = evOpen(file_data_out, "w", &evOutputHandle)) != 0)
    printf("\n ?Unable to open output file, status=%d\n\n",status);
  else
    printf("\n Successfully opened evio output file\n\n");
    
  cout<<"Kuku_0"<<endl;
  while(!feof(f))
  {
    fgets(buf, sizeof(buf), f);
    printf("Line: %s\n", buf);
  //  result = sscanf(buf, "%d %d %d %lf %lf %lf", &run_num, &event_num, &sector_num, &p, &theta, &phi);
    result = sscanf(buf, "%d", &event_num);
    printf(" result %d, event_num %d\n", result, event_num);
    if(result != 1)
      continue;

//    if(sector_num != 2)
//      continue;

    if(event_num == last_event_num)
      continue;

    last_event_num = event_num;

    while(1)
    {
      status = evRead(evInputHandle, evBuffer, EV_BUFFER_LEN);
      if(status < 0)
      {
        printf("%s - END OF EVIO FILE...\n", __func__);
        evClose(evInputHandle);
        
        exit(1);
//        
//        sprintf(buf, "%s.evio.%05d", file_data_in, evFileNumber++);
//        if((status = evOpen(buf, "r", &evInputHandle)) != 0)
//          printf("\n ?Unable to open input file %s, status=%d\n\n",buf,status);
//        else
//          printf("\n Successfully opened evio input file %s\n\n", buf);
//        status = evRead(evInputHandle, evBuffer, EV_BUFFER_LEN);
//        if(status < 0)
//          return -1;
      }
      evEventNumber++;
      
      //int fragtag = 37, fragnum = -1, banktag = 0xe10a;
      int fragtag = 40, fragnum = -1, banktag = 0xe10a;
      int ind_data, ind = 0;
      int nbytes;
      
      for(int banknum=0; banknum<40; banknum++)
      {
        ind = evLinkBank(evBuffer, fragtag, fragnum, banktag, banknum, &nbytes, &ind_data);
        if(ind > 0)
          break;
      }
      
      if(ind <= 0)
      {
        printf("ERROR: cannot find head bank\n");
        continue;
      }
      
      b08 = (unsigned char *)&evBuffer[ind_data];
      GET32(itmp);
      GET32(ievent);
      
      if(ievent == event_num)
      {
        printf("Found event numer = %d (CODA event number = %d)\n", ievent, evEventNumber);
        evWrite(evOutputHandle, evBuffer); 
        break;
      }
    }
    
  }
  evClose(evInputHandle);
  evClose(evOutputHandle);
  fclose(f);

  return 0;
}
