/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 15:32:02 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/20 16:43:16 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/** 
 * @brief locate character in string
 * @param  s: string to locate the character
 * @param  c: character to locate 
 * @retval A pointer to the matched character or NULL if the character is not found.
 */
char	*ft_strchr(const char *s, int c)
{
	unsigned int	i;
	char			*str;

	i = -1;
	str = (char *)s;
	while (str[++i])
		if (str[i] == (char)c)
			return (str + i);
	if (str[i] == (char)c)
		return (str + i);
	return (0);
}
