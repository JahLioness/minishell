/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 15:33:02 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/17 14:38:53 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <fcntl.h>
# include <limits.h>
# include <stdarg.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# ifndef MAX_OPEN_FILES
#  define MAX_OPEN_FILES 1024
# endif

/*            String Manipulation           */
int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_isascii(int c);
int			ft_isdigit(int c);
int			ft_isprint(int c);
int			ft_is_upper(int c);
void		ft_free_tab(char **tab);
char		*ft_strchr(const char *s, int c);
char		*ft_strdup(const char *s);
char		*ft_strdup_free(const char *s);
size_t		ft_strlcat(char *dest, const char *src, size_t size);
size_t		ft_strlcpy(char *dest, const char *src, size_t size);
size_t		ft_strlen(const char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strnstr(const char *big, const char *little, size_t len);
char		*ft_strrchr(const char *s, int c);
int			ft_tolower(int c);
int			ft_toupper(int c);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strtrim(char const *s1, char const *set);
char		**ft_split(char const *s, char c);
int			ft_countword(char const *s, char c);
void		ft_free_split(size_t i, char **split);
char		*ft_itoa(int n);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void		ft_striteri(char *s, void (*f)(unsigned int, char *));
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
char		*ft_strndup(char *s, size_t n);
size_t		ft_tab_len(char **tab);
char		**ft_tab_copy(char **tab);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strtok(char *str, char *sep);
char		*ft_strncpy(char *dest, const char *src, size_t n);
char		**ft_strdup_tab(char **s);
int			ft_is_whitespaces(char c);
char		*ft_strlchr(const char *s, int c, int i);

/*              Int manipulation           */
int			ft_atoi(const char *nptr);
long long	ft_atoll(char *str);
size_t		ft_nbr_length(int n);

/*              Memory manipulation        */
void		ft_bzero(void *s, size_t n);
void		*ft_calloc(size_t nmemb, size_t size);
void		*ft_memchr(const void *s, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t n);
void		*ft_memset(void *s, int c, size_t n);

/*              Printf          */
int			ft_printf(const char *format, ...);
void		ft_putnbr16(unsigned long long n, char format);
void		ft_putnbr_unsigned(unsigned long long n);
size_t		ft_unsigned_length(unsigned long long n, int base);
size_t		ft_strlen_printf(const char *s);
int			ft_deal_cformat(char c);
int			ft_deal_sformat(char *str);
int			ft_deal_pformat(uintptr_t ptr);
int			ft_deal_iformat(int nb);
int			ft_deal_uformat(unsigned int nb);
int			ft_deal_xformat(unsigned int nb, char format);

/*              Get_next_line           */
char		*get_next_line(int fd, int error);
char		*ft_join_to_nl(char *s1, char *s2);
char		*ft_strjoin_gnl(char *s1, char *s2);
char		*ft_read(int fd, char *buffer, char *next_line, ssize_t bytes_read);
char		*ft_set_line(char *next_line, char *line);
char		*ft_check_str(char *str);
size_t		ft_strlen_gnl(char *str);
char		*ft_strchr_gnl(char *s, int c);
void		ft_free_split(size_t i, char **split);

/*              List manipulation           */
// typedef struct s_list
// {
// 	void			*content;
// 	struct s_list	*next;
// }					t_list;
// t_list				*ft_lstnew(void *content);
// void				ft_lstadd_front(t_list **lst, t_list *new);
// int					ft_lstsize(t_list *lst);
// t_list				*ft_lstlast(t_list *lst);
// void				ft_lstadd_back(t_list **lst, t_list *new);
// void				ft_lstdelone(t_list *lst, void (*del)(void *));
// void				ft_lstclear(t_list **lst, void (*del)(void *));
// void				ft_lstiter(t_list *lst, void (*f)(void *));
// t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
// 						void (*del)(void *));

#endif