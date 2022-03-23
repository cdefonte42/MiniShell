/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 11:49:16 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/17 13:00:54 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "./heap.h"

uintptr_t heap[HEAP_CAP_WORDS] = {0};
const uintptr_t *stack_base = 0;

bool reachable_chunks[CHUNK_LIST_CAP] = {0};
void *to_free[CHUNK_LIST_CAP] = {0};
size_t to_free_count = 0;

Chunk_List alloced_chunks = {0};
Chunk_List freed_chunks = {
	.count = 1,
	.chunks = {[0] = {.start = heap, .size = sizeof(heap)}},
};
Chunk_List tmp_chunks = {0};

void	chunk_list_insert(Chunk_List *list, void *start, size_t size)
{
	size_t		i;
	const Chunk	t;

	assert(list->count < CHUNK_LIST_CAP);
	list->chunks[list->count].start = start;
	list->chunks[list->count].size = size;
	i = list->count;
	while (i > 0 && list->chunks[i].start < list->chunks[i - 1].start)
	{
		t = list->chunks[i];
		list->chunks[i] = list->chunks[i - 1];
		list->chunks[i - 1] = t;
		--i;
	}
	list->count += 1;
}

void	chunk_list_merge(Chunk_List *dst, const Chunk_List *src)
{
	size_t		i;
	const Chunk	chunk;
	Chunk		*top_chunk;

	i = 0;
	dst->count = 0;
	while (i < src->count)
	{
		chunk = src->chunks[i];
		if (dst->count > 0)
		{
			top_chunk = &dst->chunks[dst->count - 1];
			if (top_chunk->start + top_chunk->size == chunk.start)
				top_chunk->size += chunk.size;
			else
				chunk_list_insert(dst, chunk.start, chunk.size);
		}
		else
			chunk_list_insert(dst, chunk.start, chunk.size);
		++i;
	}
}

void	chunk_list_dump(const Chunk_List *list, const char *name)
{
	size_t	i;

	i = 0;
	printf("%s Chunks (%zu):\n", name, list->count);
	while (i < list->count)
	{
		printf("  start: %p, size: %zu\n",
			(void*) list->chunks[i].start,
			list->chunks[i].size);
		++i;
	}
}

int	chunk_list_find(const Chunk_List *list, uintptr_t *ptr)
{
	size_t	i;

	i = 0;
	while (i < list->count)
	{
		if (list->chunks[i].start == ptr)
			return ((int) i);
		++i;
	}
	return (-1);
}

void	chunk_list_remove(Chunk_List *list, size_t index)
{
	size_t	i;

	assert(index < list->count);
	i = index;
	while (i < list->count - 1)
	{
		list->chunks[i] = list->chunks[i + 1];
		++i;
	}
	list->count -= 1;
}

void	*heap_alloc(size_t size_bytes)
{
	const size_t	size_words;
	const size_t	tail_size_words;
	const Chunk		chunk;
	size_t			i;

	size_words = (size_bytes + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);
	if (size_words > 0)
	{
		chunk_list_merge(&tmp_chunks, &freed_chunks);
		freed_chunks = tmp_chunks;
		i = 0;
		while (i < freed_chunks.count)
		{
			freed_chunks.chunks[i];
			if (chunk.size >= size_words)
			{
				chunk_list_remove(&freed_chunks, i);
				tail_size_words = chunk.size - size_words;
				chunk_list_insert(&alloced_chunks, chunk.start, size_words);
				if (tail_size_words > 0)
					chunk_list_insert(&freed_chunks, chunk.start + size_words, \
					tail_size_words);
				return (chunk.start);
			}
			++i;
		}
	}
	return (NULL);
}

void	heap_free(void *ptr)
{
	const int	index;

	if (ptr != NULL)
	{
		index = chunk_list_find(&alloced_chunks, ptr);
		assert(index >= 0);
		assert(ptr == alloced_chunks.chunks[index].start);
		chunk_list_insert(&freed_chunks,
			alloced_chunks.chunks[index].start,
			alloced_chunks.chunks[index].size);
		chunk_list_remove(&alloced_chunks, (size_t) index);
	}
}

static void	mark_region(const uintptr_t *start, const uintptr_t *end)
{
	const uintptr_t	*p;
	Chunk			chunk;
	size_t			i;

	i = 0;
	while (start < end)
	{
		p = (const uintptr_t *) *start;
		while (i < alloced_chunks.count)
		{
			chunk = alloced_chunks.chunks[i];
			if (chunk.start <= p && p < chunk.start + chunk.size)
			{
				if (!reachable_chunks[i])
				{
					reachable_chunks[i] = true;
					mark_region(chunk.start, chunk.start + chunk.size);
				}
			}
			++i;
		}
		start += 1;
	}
}

void	heap_collect(void)
{
	const uintptr_t	*stack_start;
	size_t			i;

	stack_start = (const uintptr_t *)__builtin_frame_address(0);
	memset(reachable_chunks, 0, sizeof(reachable_chunks));
	mark_region(stack_start, stack_base + 1);
	to_free_count = 0;
	i = 0;
	while (i < alloced_chunks.count)
	{
		if (!reachable_chunks[i])
		{
			assert(to_free_count < CHUNK_LIST_CAP);
			to_free[to_free_count++] = alloced_chunks.chunks[i].start;
		}
		++i;
	}
	i = 0;
	while (i < to_free_count)
	{
		heap_free(to_free[i]);
	++i;
	}
}
