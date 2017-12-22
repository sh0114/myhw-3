#include <sys/types.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#include "run.h"
#include "util.h"

void *base = 0;

p_meta start = 0;
p_meta finish = 0;

p_meta find_meta(p_meta *last, size_t size) {
 
  p_meta index = base;
  p_meta result = base;
	
  switch(fit_flag){
    case FIRST_FIT:
    {
      //FIRST FIT CODE

	if(*finish == 0) 
		return 0;
	
	index = (*finish) ->prev;

	while(index)
	{
		if(index->size <size || index->free == 0)
		{
			index = index->prev;
		}
		else
		{
			result = index;
			break;
		}
	}
	
    }
    break;

    case BEST_FIT:
    {
      //BEST_FIT CODE

        if(*finish == 0)
                return 0;

        index = (*finish) ->prev;
	p_meta save = index;
	size_t sizearr[4096] = {0,};
	size_t sizemin = 1000000;

	int i , j, temp;
	for(i = 0; index != (*start) ;i++)
	{
		sizearr[i] =  index->size - size;
		index = index->prev;
	}
	
	index = save;

	for(j=0; j<=i; j++)
	{
		if(sizearr[j] > 0 && sizemin>sizearr[j])
		{
			sizemin = sizearr[j];
			j = temp;
		}
	}
	
	for(j=0;j<=i;j++)
	{
		if(j == temp)
		{
			result = index;
			break;
		}
		index = index->prev;
	}

    }
    break;

    case WORST_FIT:
    {
      //WORST_FIT CODE

	if(*finish == 0)
  	{
		return 0;
	}

        index = (*finish) ->prev;
        p_meta save = index;
        size_t sizearr[4096] = {0,};
        size_t sizemax = 0;

        int i , j, temp;
        for(i = 0; index != (*start) ;i++)
        {
                sizearr[i] =  index->size - size;
                index = index->prev;
        }

        index = save;

        for(j=0; j<=i; j++)
        {
                if(sizearr[j] > 0 && sizemax < sizearr[j])
                {
                        sizemax = sizearr[j];
                        temp = j;
                }
        }

        for(j=0;j<=i;j++)
        {
                if(j == temp)
                {
                        result = index;
                        break;
                }
                index = index->prev;
        }


    }
    break;

  }
  return result;
}

void *m_malloc(size_t size) {

	p_meta index = base;
	p_meta temp = base;
	p_meta result = base;

	if(start == 0)
	{
		index = sbrk(META_SIZE);
	
		index->free = 0;
		index-> size = size;
		index -> data[0] = sbrk(size);
		
		start = index;
		index->next = base;
		index->prev = start;

		return index->data[0];
	}
	else if(index = find_meta(&result, size))
	{
		index->free = 0;
		index->size = size;
		index->data[0] = index + META_SIZE;

		return index->data[0];	
	}
	else
	{
		index = sbrk(META_SIZE);
		index->free = 0;
		index->size = size;
		index->data[0] = sbrk(size);

		result->next = index;
		index->prev = result;
		index->next = base;
		
	}

}

void m_free(void *ptr) {

	
}

void*
m_realloc(void* ptr, size_t size)
{

}
