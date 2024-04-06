#define _CRT_SECURE_NO_WARNINGS 1
#include "driver.h"
#include <cstring>

Population *Driver(int gens) {
    Population *pop=0;
    Genome *start_genome;
    char curword[20];
    int id;

    ostringstream *fnamebuf;
    int gen;
 
    int evals[100];  
    int genes[100];
    int nodes[100];
    int winnernum;
    int winnergenes;
    int winnernodes;
    //For averaging
    int totalevals=0;
    int totalgenes=0;
    int totalnodes=0;
    int expcount;
    int samples;  //For averaging

    memset (evals, 0, NEAT::num_runs * sizeof(int));
    memset (genes, 0, NEAT::num_runs * sizeof(int));
    memset (nodes, 0, NEAT::num_runs * sizeof(int));

    ifstream iFile("genes",ios::in);

    iFile>>curword;
    iFile>>id;
    start_genome=new Genome(id,iFile);
    iFile.close();

    for(expcount=0;expcount<NEAT::num_runs;expcount++) {
      pop=new Population(start_genome,NEAT::pop_size);
            pop->verify();
      
      for (gen=1;gen<=gens;gen++) {

	//This is how to make a custom filename
	fnamebuf=new ostringstream();
	(*fnamebuf)<<"gen_"<<gen<<ends;  //needs end marker

	char temp[50];
	sprintf (temp, "gen_%d", gen);

	//Check for success
	if (Driver_Epoch(pop,gen,temp,winnernum,winnergenes,winnernodes)) {
	  evals[expcount]=NEAT::pop_size*(gen-1)+winnernum;
	  genes[expcount]=winnergenes;
	  nodes[expcount]=winnernodes;
	  gen=gens;

	}
	
	fnamebuf->clear();
	delete fnamebuf;
	
      }

      if (expcount<NEAT::num_runs-1) delete pop;
      
    }

    //Average and print stats
    cout<<"Nodes: "<<endl;
    for(expcount=0;expcount<NEAT::num_runs;expcount++) {
      cout<<nodes[expcount]<<endl;
      totalnodes+=nodes[expcount];
    }
    
    cout<<"Genes: "<<endl;
    for(expcount=0;expcount<NEAT::num_runs;expcount++) {
      cout<<genes[expcount]<<endl;
      totalgenes+=genes[expcount];
    }
    
    cout<<"Evals "<<endl;
    samples=0;
    for(expcount=0;expcount<NEAT::num_runs;expcount++) {
      cout<<evals[expcount]<<endl;
      if (evals[expcount]>0)
      {
	totalevals+=evals[expcount];
	samples++;
      }
    }

    cout<<"Failures: "<<(NEAT::num_runs-samples)<<" out of "<<NEAT::num_runs<<" runs"<<endl;
    cout<<"Average Nodes: "<<(samples>0 ? (double)totalnodes/samples : 0)<<endl;
    cout<<"Average Genes: "<<(samples>0 ? (double)totalgenes/samples : 0)<<endl;
    cout<<"Average Evals: "<<(samples>0 ? (double)totalevals/samples : 0)<<endl;

    return pop;

}

bool Driver_Evaluation(Organism *org) {
  Network *net;
  double outputs[4]; 
  double this_out; 
  int count;
  double errorsum;

  bool success;  
  int numnodes; 
		  

  int net_depth; 
  int relax; 

  double inputs[4][3]={{1.0,0.0,0.0},
		   {1.0,0.0,1.0},
		   {1.0,1.0,0.0},
		   {1.0,1.0,1.0}};
  
  net=org->net;
  numnodes=((org->gnome)->nodes).size();

  net_depth=net->max_depth();

  for(count=0;count<=3;count++) {
    net->load_sensors(inputs[count]);

    success=net->activate();

    for (relax=0;relax<=net_depth;relax++) {
      success=net->activate();
      this_out=(*(net->outputs.begin()))->activation;
    }

    outputs[count]=(*(net->outputs.begin()))->activation;

    net->flush();

  }
  
  if (success) {
    errorsum=(fabs(outputs[0])+fabs(1.0-outputs[1])+fabs(1.0-outputs[2])+fabs(outputs[3]));
    org->fitness=pow((4.0-errorsum),2);
    org->error=errorsum;
  }
  else {
    errorsum=999.0;
    org->fitness=0.001;
  }

  #ifndef NO_SCREEN_OUT
  cout<<"Org "<<(org->gnome)->genome_id<<"                                     error: "<<errorsum<<"  ["<<outputs[0]<<" "<<outputs[1]<<" "<<outputs[2]<<" "<<outputs[3]<<"]"<<endl;
  cout<<"Org "<<(org->gnome)->genome_id<<"                                     fitness: "<<org->fitness<<endl;
  #endif

  if ((outputs[0]<0.5)&&(outputs[1]>=0.5)&&(outputs[2]>=0.5)&&(outputs[3]<0.5)) {
    org->winner=true;
    return true;
  }
  else {
    org->winner=false;
    return false;
  }

}

int Driver_Epoch(Population *pop,int generation,char *filename,int &winnernum,int &winnergenes,int &winnernodes) {
  vector<Organism*>::iterator curorg;
  vector<Species*>::iterator curspecies;
  bool win=false;


  //Evaluate each organism on a test
  for(curorg=(pop->organisms).begin();curorg!=(pop->organisms).end();++curorg) {
    if (Driver_Evaluation(*curorg)) {
      win=true;
      winnernum=(*curorg)->gnome->genome_id;
      winnergenes=(*curorg)->gnome->extrons();
      winnernodes=((*curorg)->gnome->nodes).size();
      if (winnernodes==5) {
      }
    }
  }
  
  //Average and max their fitnesses for dumping to file and snapshot
  for(curspecies=(pop->species).begin();curspecies!=(pop->species).end();++curspecies) {
    (*curspecies)->compute_average_fitness();
    (*curspecies)->compute_max_fitness();
  }

  //Only print to file every print_every generations
  if  (win||
       ((generation%(NEAT::print_every))==0))
    pop->print_to_file_by_species(filename);


  if (win) {
    for(curorg=(pop->organisms).begin();curorg!=(pop->organisms).end();++curorg) {
      if ((*curorg)->winner) {
	cout<<"Best Driver #"<<((*curorg)->gnome)->genome_id<<endl;
	print_Genome_tofile((*curorg)->gnome,"best_driver");
      }
    }
    
  }

  pop->epoch(generation);

  if (win) return 1;
  else return 0;

}
