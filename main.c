/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knomura <knomura@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 16:45:08 by knomura           #+#    #+#             */
/*   Updated: 2025/11/08 18:01:29 by knomura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

typedef struct s_rule
{
	int	number_of_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_times_each_philo_eat;
}		t_rule;

int	ft_atoi(char *str)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	if (res <= 0)
	{
		printf("Error: Please enter a positive integer.\n");
		exit(1);
	}
	return (res);
}

void	insert_value(t_rule *rule, int ac, char **argv)
{
	rule->number_of_philo = ft_atoi(argv[1]);
	rule->time_to_die = ft_atoi(argv[2]);
	rule->time_to_eat = ft_atoi(argv[3]);
	rule->time_to_sleep = ft_atoi(argv[4]);
	if (ac == 5)
		rule->number_of_times_each_philo_eat = ft_atoi(argv[5]);
}

int	main(int argc, char **argv)
{
	t_rule	rule;

	if (argc != 5 && argc != 6)
	{
		printf("dame");
		return (0);
	}
	insert_value(&rule, argc - 1, argv);
	printf("%d %d %d %d", rule.number_of_philo, rule.time_to_die,
		rule.time_to_eat, rule.time_to_sleep);
	if (argc == 6)
		printf(" %d", rule.number_of_times_each_philo_eat);
	return (0);
}
