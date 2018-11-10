/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadawson <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 16:06:34 by jadawson          #+#    #+#             */
/*   Updated: 2018/11/10 13:36:55 by jadawson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*connect(char *str, char *holder) //connects str to the buf
{
	printf("connect statement\n");
	char *strptr;

	strptr = ft_strjoin(str, holder);
	free(str);
	return (strptr);
}

int		the_end(char **line, char **strings, char **holder) //last section we gotta deal withm.
{
	if (ft_strlen(strings[0]) != 0)
	{
		printf("the_end if statement\n");
		*line = strings[0];
		free(*holder);
		*holder = ft_strnew(0);
		return (1);
	}
	free(*holder);
	free(strings[0]);
	strings[0] = NULL;
	*holder = NULL;
	return (0);
}

int		looping(int fd, char **strings, char **holder, char **line)
{
	int		ret;
	char	buf[BUFF_SIZE + 1]; //allocate space for buff.

	while (1)
	{
		ret = read(fd, &buf, BUFF_SIZE);
		if (ret < 0)
			return (-1);
		if (ret == 0)
			return (the_end(line, strings, holder));
		buf[ret] = '\0';
		strings[1] = ft_strchr(buf, '\n');
		if (strings[1] != NULL)
		{
			printf("looping if statement\n");
			*strings[1] = '\0';
			strings[0] = connect(strings[0], buf);
			*strings[1] = '\n';
			*holder = connect(*holder, strings[1] + 1);
			*line = strings[0];
			printf("holder in looping %p\n", *holder);
			printf("holder again \"%s\"\n", *holder);
			return (1);
		}
		free(strings[1]); //testing. did not work.
		strings[0] = connect(strings[0], buf);
	}
}

int		get_next_line(const int fd, char **line) //start
{
	char		*strings[3];
	static char	*holder;

	strings[0] = ft_strnew(0);
	if (holder == NULL)
		holder = ft_strnew(0);
	if (fd < 0 || !line || BUFF_SIZE < 0)
		return (-1);
	strings[1] = ft_strchr(holder, '\n');
	if (strings[1] != NULL)
	{
		*strings[1] = '\0';
		*line = ft_strdup(holder);
		*strings[1] = '\n';
		strings[2] = ft_strdup(strings[1] + 1);
		printf("this line \"%s\"", strings[2]);//test
		if (holder)
			free(holder);
		holder = strings[2];
		free(strings[0]);
		return (1);
	}
	if (holder[0] != '\n')
		strings[0] = connect(strings[0], holder);//leaks if it's a \n
	else
		free(strings[0]);
	printf("strings[0] after gnl if statement %p\n", strings[0]);
	if (holder)	
		free(holder);
	holder = ft_strnew(0);
	return (looping(fd, strings, &holder, line));
}
