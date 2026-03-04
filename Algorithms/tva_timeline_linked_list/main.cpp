/*
Description: Loki timeline simulator thing, clever uses of linkedlists
Input: use rng or input "./a.out <seed> <nexus events> <starting year> <timeline length>"
Output: original timeline, nexus events (visualized), and actions of fixing those events
*/

#include<iostream>
using namespace std;

class node{
public:
  //Your Code Here
  //3 vars: integers for year and branches, and next pointer of node type
  int year;
  int branches;
  node * next;
  node()     //Default Constructor
  {
    year = 0;
    branches = 0;
    next = nullptr;
  }
  node(int year)  //Constructor with year as arg
  {
    this->year = year;
    branches = 0;
    next = nullptr;
  }
};

class Timeline{
  public:
    //Your Code Here
    Timeline(int year = 0, int size = 0) //Constructor with start year and size. default param are 0 for both.
    {
      head = new node(year);
      this->size = size;
      node * current = head;
      if(year > 0)
      {
        // populate timeline with years
        for(int i = 1; i < size; i++)
        {
          current->next = new node(year+i);
          current = current->next;
        }
      }
    }
    int createNexusEvent(int amount = 1){
      if(!head) return 0; //Can't create nexus event if timeline is empty!
      for(int i=0; i< amount; i++){
        int year = (rand() % size) + head->year;
        //branchsize is MaxSize minus years left give us how big branch can be before end of time 
        int branchSize = rand() % (head->year+size-year)/4; //divide by 4 to keep them smaller for visual reasons

        //Rest of function here. Don't change above so the rands match codegrade.

        //Your code Here for createNexusEvent

        // create a node utilizing bracket technique mentioned in the pdf
        node * split = new node[2];
        // create a node for the head since you don't want to change the actual value of the head 
        //(typically you always want to do this when you are altering nodes!!!!!!!)
        node * temp = head;
        // have a while loop thats checks if the node you are on is not 
        // equal to the year that is randomly generated AND if the node you are on doesn't have a node after it
        while(temp->year != year && temp->next != nullptr)
        {
        // inside that loop, you want to set the node you are on equal to the node after it
          temp = temp->next;
        }
        // You then want to make the new timeline (hint: this is where 
        // you utilize the node with indexes. Use the head to create these timelines)
        if(temp->next != nullptr)
        {
          split[0].year = temp->next->year;
          split[0].next = temp->next->next;
          split[0].branches = temp->next->branches;
        }
        
        if(temp->branches == 0)
        {
          delete temp->next;
        }
        else if (temp->branches == 1)
        {
          delete[] temp->next;
        }
        
        temp->next = nullptr;
        temp->next = split;
        // update the timeline branches
        temp->branches=1;
        // create the second timeline (hint: this timeline wont have branches)
        // To alter the second timeline create a temp node and set it equal to &<nodeName>[1]
        split[1].year = temp->year;
        node * ptr = &split[1];
        // expand the rest of the second timeline kinda like how you created the list in the constructor
        for(int j = 0; j < branchSize; j++)
        {
          ptr->next = new node(year+j+1);
          ptr = ptr->next;
        }
        ptr->next = nullptr;

        cout << "Created Nexus at " << year << ", Size: " << branchSize+1 << endl;
      }
      return amount;
    }

    void print(){
      cout << "The Sacred Timeline\ns->";
      //Your Code Here for print. It should match the output in wtf function exactly.
      node * midptr = head;
      //output head node
      cout << midptr->year << "->";
      while(midptr->next != nullptr)
      {
        if(midptr->branches == 0)
        {
          cout << midptr->next->year;
          cout << "->";
        }
        else if(midptr->branches == 1)
        {
          node * temp = &(midptr->next[1]);
          cout << "(";
          // cout << "*" << temp->year << ".1" << "->";
          // cout << "\nmidptr[1].year = " << midptr[1].year << endl;
          // cout << "\nmidptr[1].next-> year = " << temp->year << endl;
          while(temp != nullptr)
          {
            cout << "*" << temp->year << ".1";
            if(temp->next == nullptr)
            {
              cout << ")~>";
            }
            else
            {
              cout << "->";
            }
            temp = temp->next;
            
          }
          cout << midptr->next->year;
          cout << "->";
        }
        midptr = midptr->next;
      }
      cout << "x\n" << endl;
    }
    // Visual Print Mode
    void vprint(){
      //Your Code Here for visual print.
      //Best thing to do is to create a 2D array of chars. generate the proper drawing,
      char arr[11][70];
      // initialize all to whitespace
      for(int i = 0; i < 11; i++) 
      {
        for(int j = 0; j < 70; j++)
        {
          arr[i][j] = ' ';
        }
      }
      // fill top and bottom of vprint
      for(int j = 0; j < 70; j++) 
      {
        arr[0][j] = '#';
        arr[10][j] = '#';
      }
      // fill middle area
      for(int j = 0; j < size && j < 70; j++)
      {
        arr[5][j] = '-';
      }


      node * iter = head; // for traversing main timeline to find branches
      node * branchIter; // for traversing branches and printing ~
      int yearCounter = 0; // to be able to write to right spot in arr
      bool printTop = true; // to decide whether to print above or below line

      while(iter != nullptr && yearCounter < 70)
      {
        if(iter->branches == 1) // year with branch found
        {
          branchIter = &(iter->next[1]);
          if(printTop == true)
          {
            arr[4][yearCounter] = '/';
            arr[3][yearCounter+1] = '/';
            for(int i = 0; branchIter != nullptr; i++)
            {
              if((yearCounter + 2 + i) < 70)
              {
                arr[2][yearCounter + 2 + i] = '~';
              }
              branchIter = branchIter->next;
            }
            printTop = false;
          }
          else
          {
            arr[6][yearCounter] = '\\';
            arr[7][yearCounter+1] = '\\';
            for(int i = 0; branchIter != nullptr; i++)
            {
              if((yearCounter + 2 + i) < 70)
              {
                arr[8][yearCounter + 2 + i] = '~';
              }
              branchIter = branchIter->next;
            }
            printTop = true;
          }
        }
        iter = iter->next;
        yearCounter++;
      }

      //and then print out the array in a loop at the end. do 11 by 70.
      for(int i = 0; i < 11; i++)
      {
        for(int j = 0; j < 70; j++)
        {
          cout << arr[i][j];
        }
        cout << endl;
      }
      //branches alternate, first up, then down, then up, then down, and so on.
      //they always have two slashes in appropriate direction and then ~ where each ~ represents a branched node.
      //in the sacred timeline each - represents 1 year. If 2 branches happen to overlap it's ok don't worry about it.
      //same thing if the map is too wide (bigger than 70) don't worry about it just stop printing and cut it off so
      //it does not segfault
    }
    int prune(){ //prunes a branch and returns location it pruned. For all time. always.
      int yearWithBranch = -1;
      int branchSize = 1;
      //Your Code Here
      node * iter = head;
      while(iter->next != nullptr) // loop until the next node doesn't exist
      {
        if(iter->branches == 1)
        {
          iter->branches = 0; // update the branch to show as removed
          yearWithBranch = iter->year; // update the yearwithbranch
          node * dealloc = iter->next[1].next; //
          node * branchIter = iter->next[1].next;
          while(branchIter != nullptr)
          {
            branchIter = branchIter->next;
            delete dealloc;
            dealloc = nullptr;
            dealloc = branchIter;
            branchSize++;
          }
          node * sacred = new node;
          sacred->next = iter->next[0].next;
          sacred->year = iter->next[0].year;
          sacred->branches = iter->next[0].branches;
          delete[] iter->next;
          iter->next = nullptr;
          iter->next = sacred;
          // delete midPtr->next;
          // iter->next = stored;
          // yearWithBranch = stored->year;
          // delete pruneLine;
          break;
        }

        iter = iter->next; //set next to new head

      }


      if(yearWithBranch != -1)
      {
        cout << "Pruned Brach at: " << yearWithBranch << " of size " << branchSize << endl;
      }
      return yearWithBranch;
    }

    ~Timeline()
    {
      //Your Code Here for de-allocating. if you find a branch that was not pruned just cout that it was not pruned.
        while(head != nullptr) // loop until the next node doesn't exist
      {
        // if(head->branches > 0)
        // {
        //   cout << "Branch was not pruned!" << endl; // may need updating
        // } 
        node * temp = head; // make temp ptr for head
        head = head->next; //set next to new head
        delete temp; // delete the temp
      }
    }
  //Don't modify any code beneath this line or the TVA will prune you!!!
  private:
    int size;
    node *head;
  public:
    void wtf(){ //This code should print your sacred timeline identical to your print function//
{int o_384d4468adeab2d9aed6871 = 121934831;};typedef node o_384d4468adeab2d9aed6871e0fa;
cout<<"\x54""h\145 \x53""a\143r\x65""d\040T\x69""m\145l\x69""n\145"<<endl;
o_384d4468adeab2d9aed6871e0fa* o_384d4468adeab2d9aed6871e0fa887d6=head;cout<<"\x73""-\076";
while (o_384d4468adeab2d9aed6871e0fa887d6){cout<<o_384d4468adeab2d9aed6871e0fa887d6->year
<<"\x2D"">";if ((o_384d4468adeab2d9aed6871e0fa887d6->branches > (0x0000000000000000 + 
0x0000000000000200 + 0x0000000000000800 - 0x0000000000000A00)) & !!(o_384d4468adeab2d9aed6871e0fa887d6->
branches > (0x0000000000000000 + 0x0000000000000200 + 0x0000000000000800 - 0x0000000000000A00)))
{cout<<"\x28""";for (int o_8aec06cef502177edb2e9beb8e48618e=(0x0000000000000000 + 0x0000000000000200 + 
0x0000000000000800 - 0x0000000000000A00);(o_8aec06cef502177edb2e9beb8e48618e < o_384d4468adeab2d9aed6871e0fa887d6->
branches) & !!(o_8aec06cef502177edb2e9beb8e48618e < o_384d4468adeab2d9aed6871e0fa887d6->branches);
o_8aec06cef502177edb2e9beb8e48618e++){cout<<"\x2A"""<<o_384d4468adeab2d9aed6871e0fa887d6->
next[o_8aec06cef502177edb2e9beb8e48618e + (0x0000000000000002 + 0x0000000000000201 + 0x0000000000000801 - 
0x0000000000000A03)].year<<"\x2E"""<<o_8aec06cef502177edb2e9beb8e48618e + (0x0000000000000002 + 
0x0000000000000201 + 0x0000000000000801 - 0x0000000000000A03);o_384d4468adeab2d9aed6871e0fa* 
o_c4e691df50ffe930c3969e01258ea6a1=o_384d4468adeab2d9aed6871e0fa887d6->next[o_8aec06cef502177edb2e9beb8e48618e + 
(0x0000000000000002 + 0x0000000000000201 + 0x0000000000000801 - 0x0000000000000A03)].next;
while (o_c4e691df50ffe930c3969e01258ea6a1){cout<<"\x2D"">\052"<<o_c4e691df50ffe930c3969e01258ea6a1->
year<<"\x2E"""<<o_8aec06cef502177edb2e9beb8e48618e + (0x0000000000000002 + 0x0000000000000201 + 
0x0000000000000801 - 0x0000000000000A03);o_c4e691df50ffe930c3969e01258ea6a1 = o_c4e691df50ffe930c3969e01258ea6a1->
next;};};cout<<"\x29""~\076";};o_384d4468adeab2d9aed6871e0fa887d6 = o_384d4468adeab2d9aed6871e0fa887d6->next;};
cout<<"\x78""\x0A"<<endl;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    }
};

//FOR ALL TIME. ALWAYS
int main(int argc, char *argv[]) { // ./a.out 40 4 1970 50 (40 is seed, 4 is number of nexus events to 
  //create 1970 is starting year of the sacred timeline, and 50 is the length of the sacred timeline).
  int seed = 40;                //for testing with no cmd args
  int nexusEventsToCreate = 4;  //for testing with no cmd args
  int startYear = 1970;         //for testing with no cmd args
  int timelineLength = 50;      //for testing with no cmd args
  if(argc == 5){ //for codegrading
    seed = atoi(argv[1]);
    nexusEventsToCreate = atoi(argv[2]);
    startYear = atoi(argv[3]);
    timelineLength = atoi(argv[4]);
  }
  srand(seed);
  Timeline sacredTimeline(startYear, timelineLength);
  sacredTimeline.print();
  sacredTimeline.createNexusEvent(nexusEventsToCreate);
  // sacredTimeline.print();
  sacredTimeline.wtf();
  sacredTimeline.vprint();
  cout << "Nexus Event Detected! Sending Minute Men to Prune the Branches!\n" << endl;
  //on behalf of the time variance authority, I hereby arrest you for crimes against the sacred timeline
  while(sacredTimeline.prune()!=-1){
    sacredTimeline.print();
  }
  cout << "\nSacred Timeline Restored :)" << endl;
  sacredTimeline.print(); 
  sacredTimeline.vprint();

  cout << "___________   ____\n| /|    \\  \\ /  / \\\n|/ |    |\\  Y \\/   \\\n   "
       <<   "|    | \\   /\\    \\\n   |____|  \\_/_/\\____\\\n FOR ALL TIME.  ALWAYS.\n"<< endl;
  return 0;
}