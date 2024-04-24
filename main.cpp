//
//  main.cpp
//  doubly linkedlist basics
//
//  Created by bk on 4/24/24.
//

#include <iostream>
#include <stdio.h>

// note, not entirely safe for passing in NULLs
// do w/ old fashioned allocs new/delete for now

class ListItem {

    public:
    
    ListItem * priorItem = NULL;
    ListItem * nextItem = NULL;
    
    void * applicationData = NULL;

    ListItem(void * applicationDataInput) {
       priorItem = NULL;
       nextItem = NULL;
       applicationData = applicationDataInput;
    }
    
    ListItem(ListItem * priorItemInput, ListItem * nextItemInput, void * applicationDataInput) {
        priorItem = priorItemInput;
        nextItem = nextItemInput;
        applicationData = applicationDataInput;
    }
    
};

// caller should manage mem of items

class LinkedList {

    public:
    
    ListItem * firstItem = NULL;
    ListItem * currentItem = NULL;
    
    // constructor creates an empty linked list
    
    LinkedList() {
      firstItem = NULL;
      currentItem = NULL;
    }

    ListItem * getCurrentItem() {
      if (!currentItem) currentItem = firstItem;
      return currentItem;
    }
    
    void addToEnd(ListItem * a) {
    
       ListItem * lastItem = end();
       
       if (lastItem)
         printf("add to end, last item app data = %s\n", (char*)lastItem->applicationData);
       
       if (lastItem) {
       
         lastItem->nextItem = a;
         
         a->priorItem = lastItem;
         a->nextItem = NULL;
         
       }
       else {
         // no last item just add to beginning
         printf("no last item just add to beginning\n");
         addToBeginning(a);
       }
       
       currentItem = a;
       
    }
    
    void addToBeginning(ListItem * a) {
    
       ListItem * hold = firstItem;
       
       firstItem = a;
       
       firstItem->priorItem = NULL;
       firstItem->nextItem = hold;
       
       if (hold) hold->priorItem = firstItem;
       
    }
    
    void insertAfter(ListItem * left, ListItem * toInsert) {
       
       // edge case handle w/ predefined method
       
       if (left == end()) {
          addToEnd(toInsert);
          return;
       }
    
       ListItem * found = find(left);
       
       if (!found) {
         printf("cannot find item %lx with app data %s, cannot insert new item\n", left, (char*)(left->applicationData));
       } else {
       
         printf("found item at left, inserting after\n");
         
         ListItem * rightItem = found->nextItem;
         
         printf("rightItem = %lx %s\n", rightItem, (char*)rightItem->applicationData);
         
         ListItem * pointerToPriorLeftItem = rightItem->priorItem;
         
         toInsert->priorItem = found;
         
         toInsert->nextItem = rightItem;
         
         rightItem->priorItem = toInsert;
       
         found->nextItem = toInsert;
       
       }
       
    }
    
    void removeItem(ListItem * toRemove) {
    
       ListItem * found = find(toRemove);
       
       bool isEnd = false;
       if (toRemove == end()) isEnd = true;
       
       if (found) {
       
           ListItem * leftItem = found->priorItem;
           ListItem * rightItem = found->nextItem;
           
           // short circuit past this one

           if (leftItem) {
             leftItem->nextItem = isEnd ? NULL : rightItem;
             
           }
           
           if (rightItem)
             rightItem->priorItem = leftItem;
             
           if (toRemove == firstItem) firstItem = rightItem;
           
       }
           
    }
    
    // simple find scroll thru not optimized
    // return NULL if not found
    
    ListItem * find(ListItem * a) {
       currentItem = firstItem;
       
       if (currentItem == a) return currentItem;
       
       while (next()) {
         if (currentItem == a) return currentItem;
       }
       
       return currentItem;
    }
    
    ListItem * next() {
        ListItem * tentativeCurrentItem = getCurrentItem();
        currentItem = tentativeCurrentItem ? tentativeCurrentItem->nextItem : NULL;
        return currentItem;
    }
    
    ListItem * previous() {
        currentItem = getCurrentItem()->priorItem;
        return currentItem;
    }
    
    // find end of list the slow way
    ListItem * end() {
       ListItem * tentativeEnd = firstItem;
       do {
        if (currentItem) tentativeEnd = currentItem;
       } while(next());
       return tentativeEnd;
    }
    
    // audit whole list printing out application data for each list item
 
    // test scroll fwd thru list w/ next function
     
    void audit() {
    
       ListItem * savedCurrentItem = currentItem;
       
       currentItem = firstItem;
    
       // assume application data is a char * type string for this audit
       
       int iteration = 0;
       
       printf("%d %s\n", iteration, (char*)currentItem->applicationData);
       
       while(next()) {
       
         iteration++;
         printf("%d %s\n", iteration, (char**)currentItem->applicationData);
        
       }
       
       currentItem = savedCurrentItem;
       
    }
    
    // test scroll backwd w/ previous() function
    
    void auditbackwards() {
    
       printf("audit list scroll backwards:\n");
       
       ListItem * savedCurrentItem = currentItem;
       
       currentItem = end();
    
       printf("found end app data = %s\n", ((char*)currentItem->applicationData));
       
       // assume application data is a char * type string for this audit
       
       int iteration = 0;
       
       // previous() changes currentItem to reflect the "previous" operation
       
       do {
       
         printf("%d %s\n", iteration, (char**)currentItem->applicationData);

         iteration++;
        
       } while(previous());
       
       currentItem = savedCurrentItem;
       
    }
    
    
};

int main(int argc, const char * argv[]) {

    printf("linked list tests\n");
    
    ListItem * a = new ListItem((void*)"A");
    ListItem * b = new ListItem((void*)"B");
    ListItem * c = new ListItem((void*)"C");
    ListItem * d = new ListItem((void*)"D");
    ListItem * e = new ListItem((void*)"E");
    
    LinkedList * list1 = new LinkedList();
    
    // test add to beginning
    
    list1->addToBeginning(a);
    
    // note:  audit() forward-scrolls thru list from beginning to end, testing the next() func
    // auditbackwards() [used later] starts at the end and goes backwards to get beginning, testing previous() func
    
    list1->audit();
    
    printf("prior should be a list of 1 item A\n");
    
    printf("---\n");
    
    list1->addToBeginning(b);
    list1->addToBeginning(c);
    
    // test add to end of an existing list
    
    list1->addToEnd(d);
    
    // then test add to beginning of that list
    
    list1->addToBeginning(e);
    
    list1->audit();
    
    printf("prior should be ECBAD\n");
    
    // should be ECBAD
    
    printf("---\n");
    
    printf("list 2 tests\n");
    
    LinkedList * list2 = new LinkedList();
    
    // test add to end on empty list
    
    list2->addToEnd(a);
    
    list2->audit();
    
    printf("prior should be A\n");
    
    // test add to end of existing list
    
    list2->addToEnd(b);
    
    list2->audit();
    
    printf("prior should be AB\n");
       
    // test add to end of existing list longer list
      
    list2->addToEnd(c);
    
    list2->audit();
     
    printf("prior should be ABC\n"); // better
     
    // test add to end of existing list longer list
    
    list2->addToEnd(d);
    
    list2->audit();
    
    printf("---\n");
    
    printf("prior should be ABCD\n"); // better
    
    // now test add to beginning of this list
    
    list2->addToBeginning(e);
    
    list2->audit();
    
    printf("---\n");
      
    printf("prior should be EABCD\n"); // better
     
    list2->auditbackwards();
    
    printf("prior should be DCBAE\n"); // not working
     
    // test find item in middle of list
    
    ListItem * tryToFindB = list2->find(b);
    printf("found b %lx with app data %s\n", tryToFindB, (char*)(tryToFindB->applicationData));
    
    ListItem * tryToFindC = list2->find(c);
    printf("found c %lx with app data %s\n", tryToFindC, (char*)(tryToFindC->applicationData));

    // test find at beginning of list edge case
    ListItem * tryToFindD = list2->find(d);
    printf("found d %lx with app data %s\n", tryToFindD, (char*)(tryToFindD->applicationData));
    
    // test find at end of list edge case
    ListItem * tryToFindA = list2->find(a);
    printf("found a %lx with app data %s\n", tryToFindA, (char*)(tryToFindA->applicationData));
   
    // try to find an item that is not in the list
    
    ListItem * outerspace = new ListItem((void*)"OUTERSPACE");
    
    ListItem * testFind = list2->find(outerspace);
    
    printf("testFind = %lx should be 0 or NULL\n", testFind);

    // middle of list insert test
    
    // find item C then insert after it
    
    {
        ListItem * tryToFindC = list2->find(c);
        
        printf("found c %lx with app data %s\n", tryToFindC, (char*)(tryToFindC->applicationData));
        
         ListItem * afterC = new ListItem((void*)"afterC");
         
        if (tryToFindC)
          list2->insertAfter(tryToFindC, afterC);
        else
          printf("did not find C could not insert after\n");
       
        list2->audit();
        
        printf("above should be EABC afterC D\n");
        
        printf("---\n");
        
    }
     
    // edge case test
    // find item D (last item from prior tests) then insert after it
    
    ListItem * afterD = new ListItem((void*)"afterD");
    
    {
        ListItem * tryToFindD = list2->find(d);
        
        printf("found d %lx with app data %s\n", tryToFindD, (char*)(tryToFindD->applicationData));
        
        if (tryToFindD)
          list2->insertAfter(tryToFindD, afterD);
        else
          printf("did not find D could not insert after\n");
       
        list2->audit();
        
        printf("above should be EABC afterC D afterD\n");
        
        printf("---\n");
        
        
    }
    
    // edge case test
    // find item D (first item from prior tests) then insert after it
    
    {
        ListItem * tryToFindE = list2->find(e);
        
        printf("found e %lx with app data %s\n", tryToFindE, (char*)(tryToFindE->applicationData));
        
         ListItem * afterE = new ListItem((void*)"afterE");
         
        if (tryToFindE)
          list2->insertAfter(tryToFindE, afterE);
        else
          printf("did not find E could not insert after\n");
       
        list2->audit();
        
        printf("above should be E afterE ABC afterC D afterD\n");
        
        printf("---\n");
        
        list2->auditbackwards();
        
        printf("above should be afterD D afterC CBA afterE E\n"); // broken, prev links in inserted items seem wrong
        
        printf("---\n");
           
    }
    
    // test remove item in middle of list
    
    list2->removeItem(a);
    
    list2->auditbackwards();
     
    printf("above should be afterD D afterC CB afterE E\n");

    list2->audit();
     
    printf("above should be E afterE BC afterC D afterD\n");
     
    // test remove item at beginning of list
     
    list2->removeItem(e);
    
    list2->audit();
     
    printf("above should be afterE BC afterC D afterD\n");

    list2->auditbackwards();
     
    printf("above should be afterD D afterC CB afterE\n");
    
    // test remove item at end of list
    
    list2->removeItem(afterD);
    
    list2->audit();
     
    printf("above should be afterE BC afterC D\n");
    
    list2->auditbackwards();
     
    printf("above should be D afterC CB afterE\n");
    
    // since this is the end of main, don't need to delete the items alloc'd by new, process cleanup handles it
    // normally we would have a delete for every new
    
    return 0;
}


