/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 10:52:49 by juramos           #+#    #+#             */
/*   Updated: 2024/04/03 11:22:40 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// strjoin with free is a regular strjoin with the particularity
// that frees s1 after its use.
char	*ft_strjoin_with_free(char *s1, char const *s2)
{
	char	*s3;
	int		pos;
	int		helper;

	pos = 0;
	helper = 0;
	if (!s1 || !s2)
		return (0);
	s3 = ft_calloc((ft_strlen(s1) + ft_strlen(s2) + 1), sizeof(char));
	if (!s3)
		return (0);
	while (s1[helper] != '\0')
		s3[pos++] = s1[helper++];
	helper = 0;
	while (s2[helper] != '\0')
		s3[pos++] = s2[helper++];
	free(s1);
	return (s3);
}

// returns everything found until first linebreak or end of file.
// initializes ret if it doesn´t exist, otherwise appends to
// remains of previous read, which happens when filelen % buffer_size != 0.
char	*ft_read_file(int fd, char	*ret)
{
	char	*buffer;
	int		bytes_read;

	if (!ret)
		ret = ft_calloc(1, sizeof(char));
	buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!ret || !buffer)
		return (NULL);
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buffer);
			free(ret);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		ret = ft_strjoin_with_free(ret, buffer);
		if (ft_strchr(ret, '\n'))
			break ;
	}
	free(buffer);
	return (ret);
}

// will read everything the file_content string contains
// until the first linebreak or end of file, and will return
// everything up to that point, including the linebreak/eof.
char	*ft_read_top_line(char *file_content)
{
	int		line_len;
	char	*top_line;

	line_len = 0;
	while (file_content[line_len] != '\n'
		&& file_content[line_len] != '\0')
		line_len++;
	top_line = ft_substr(file_content, 0, line_len + 1);
	return (top_line);
}

// receives a str file_content and returns a new one,
// starting at the next position from the first linebreak
// and freeing the str it receives.
char	*ft_remove_top_line(char *file_content)
{
	int		file_len;
	int		start_of_next;
	char	*next_file_content;

	file_len = ft_strlen(file_content);
	if (!file_content || file_len == 0)
	{
		free(file_content);
		return (NULL);
	}
	start_of_next = 0;
	while (file_content[start_of_next] != '\n'
		&& file_content[start_of_next] != '\0')
		start_of_next++;
	start_of_next++;
	next_file_content = ft_substr(file_content, start_of_next, file_len);
	free(file_content);
	return (next_file_content);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = ft_read_file(fd, buffer);
	if (!buffer)
		return (NULL);
	line = ft_read_top_line(buffer);
	buffer = ft_remove_top_line(buffer);
	if (!buffer)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

// int	main(void)
// {
// 	int		fd;
// 	char	*current_line;

// 	fd = open("hello.txt", O_RDONLY);
// 	current_line = get_next_line(0);
// 	while (current_line)
// 	{
// 		printf("%s", current_line);
// 		free(current_line);
// 		current_line = get_next_line(fd);
// 	}
// 	free(current_line);
// 	close(fd);
// 	system("leaks -q a.out");
// 	return (0);
// }
