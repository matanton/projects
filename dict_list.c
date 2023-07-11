#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "Clibrary/get_next_line/get_next_line.h"

typedef struct s_list
{
	char *key;
	char *value;
	struct s_list *next;
} t_list;

char **ft_split(char const *s, char c);

size_t ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t i;
	size_t srcsize;

	srcsize = ft_strlen(src);
	i = 0;
	if (dstsize != 0)
	{
		while (src[i] != '\0' && i < (dstsize - 1))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (srcsize);
}

void *ft_memcpy(void *dst, const void *src, size_t n)
{
	if (!(dst == NULL && src == NULL) && n > 0)
	{
		while (n--)
			((char *)dst)[n] = ((char *)src)[n];
	}
	return (dst);
}

void *ft_memset(void *b, int c, size_t len)
{
	size_t i;

	i = 0;
	while (len--)
	{
		((char *)b)[i] = c;
		i++;
	}
	return (b);
}

void ft_bzero(void *s, size_t n)
{
	ft_memset(s, '\0', n);
}

void *ft_calloc(size_t count, size_t size)
{
	size_t n;
	void *p;

	if (count == 0 || size == 0)
	{
		count = 1;
		size = 1;
	}
	n = count * size;
	p = malloc(n);
	if (p == NULL)
		return (NULL);
	ft_bzero(p, n);
	return (p);
}

char *ft_strdup(const char *s1)
{
	char *p;
	int i;

	p = ((char *)ft_calloc(sizeof(char), (ft_strlen(s1) + 1)));
	i = 0;
	if (!p)
		return (NULL);
	ft_memcpy(p, s1, ft_strlen(s1));
	return (p);
}

char *ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t i;
	char *p;
	size_t s_len;

	i = 0;
	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if (s_len - start >= len)
		p = malloc(len + 1);
	else
		p = malloc((s_len - start) + 1);
	if (!p)
		return (NULL);
	ft_strlcpy(p, s + start, (len + 1));
	return (p);
}

char *ft_strtrim(char const *s1, char const *set)
{
	size_t s1size;
	char *n;

	if (!s1 || !set)
		return (NULL);
	while (*s1 && ft_strchr(set, *s1))
		s1++;
	s1size = ft_strlen(s1);
	while (s1size && ft_strchr(set, s1[s1size]))
		s1size--;
	n = ft_substr((char *)s1, 0, s1size + 1);
	return (n);
}

static size_t ft_counter(char const *str, char c)
{
	int i;
	int words;

	i = 0;
	words = 0;
	while (str[i] != '\0')
	{
		if (str[i] != c && (str[i - 1] == c || i == 0))
			words++;
		i++;
	}
	return (words);
}

static int ft_len(char const *str, char c)
{
	int count;

	count = 0;
	while (str[count] != c && str[count] != '\0')
		count++;
	return (count);
}

static char *ft_copy(char const *s, char c)
{
	int i;
	int count;
	char *copy;

	i = 0;
	count = 0;
	while (s[count] != '\0' && s[count] != c)
		count++;
	copy = malloc(sizeof(char) * (count + 1));
	if (!copy)
		return (NULL);
	while (i < count)
	{
		copy[i] = s[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

char **ft_split(char const *s, char c)
{
	size_t i;
	size_t j;
	int t;
	char **split;

	i = 0;
	j = 0;
	if (!s)
		return (NULL);
	split = malloc(sizeof(char *) * (ft_counter(s, c) + 1));
	if (!split)
		return (NULL);
	while (j < ft_counter(s, c))
	{
		while (s[i] && s[i] == c)
			i++;
		t = ft_len(&s[i], c);
		split[j] = ft_copy(s + i, c);
		j++;
		i = i + ft_len(&s[i], c);
	}
	split[j] = NULL;
	return (split);
}

t_list *ft_lstnew(char *key, char *word)
{
	t_list *node;

	node = malloc(sizeof(t_list));
	if (node != NULL)
	{
		node->key = key;
		node->value = word;
		node->next = NULL;
	}
	return (node);
}

t_list *ft_lstlast(t_list *list)
{
	t_list *tmp;

	tmp = list;
	if (list != NULL)
	{
		while (tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		return (tmp);
	}
	else
		return (NULL);
}

void ft_lstadd_back(t_list **list, t_list *new)
{
	t_list *tmp;

	if (new == NULL)
		return;
	if (*list != NULL)
	{
		tmp = ft_lstlast(*list);
		tmp->next = new;
	}
	else
		*list = new;
}

t_list *dict_list(void)
{
	int fd;
	char *temp;
	char **split;
	t_list *list;

	fd = open("numbers.dict", O_RDONLY);
	list = NULL;
	if (fd < 0)
		return (NULL);
	temp = get_next_line(fd);
	while (temp != NULL)
	{
		split = ft_split(temp, ':');
		ft_lstadd_back(&list, ft_lstnew(split[0], ft_strtrim(split[1], " \n")));
		temp = get_next_line(fd);
	}
	return (list);
}

int main(void)
{
	t_list *list = dict_list();
	while (list != NULL)
	{
		printf("key: [%s] --> content: [%s]\n", list->key, list->value);
		list = list->next;
	}
	return 0;
}