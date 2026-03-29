/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knomura <knomura@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 16:45:08 by knomura           #+#    #+#             */
/*   Updated: 2026/03/29 15:40:09 by knomura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	t_ms(void)
{
	struct timeval	time;
	long			start;

	gettimeofday(&time, NULL);
	start = time.tv_sec * 1000000 + time.tv_usec;
	return (start);
}

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

void	*check_death(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		for (int i = 0; i < data->rule_data.number_of_philo; i++)
		{
			// printf("checking %d philo last eat time\n", i);
			// printf("%ld > %d\n", (t_ms()
			// - data->philos_data[i].last_eat_time )/ 1000,
			// data->rule_data.time_to_die / 1000);
			if (t_ms()
				- data->philos_data[i].last_eat_time > data->rule_data.time_to_die)
			{
				printf("%ld %d died\n", (t_ms() - data->rule_data.start_time)
					/ 1000, data->philos_data[i].id);
				return (NULL);
			}
		}
		usleep(100);
	}
}

void	*routine(void *arg)
{
	t_philo	*p;
	t_data	*data;
	t_rule	rule;

	p = (t_philo *)arg;
	data = p->data;
	rule = data->rule_data;
	while (1)
	{
		if (p->id % 2 == 0)
		{
			pthread_mutex_lock(p->right_fork);
			pthread_mutex_lock(p->left_fork);
			printf("%ld %d has taken a fork\n", (t_ms() - rule.start_time)
				/ 1000, p->id);
			printf("%ld %d is eating\n", (t_ms() - rule.start_time) / 1000,
				p->id);
			usleep(rule.time_to_eat);
			p->last_eat_time = t_ms();
			pthread_mutex_unlock(p->right_fork);
			pthread_mutex_unlock(p->left_fork);
			p->meal_count++;
			if (p->meal_count == rule.number_of_times_each_philo_eat)
				return (NULL);
			printf("%ld %d is sleeping\n", (t_ms() - rule.start_time) / 1000,
				p->id);
			usleep(rule.time_to_sleep);
			printf("%ld %d is thinking\n", (t_ms() - rule.start_time) / 1000,
				p->id);
		}
		else
		{
			pthread_mutex_lock(p->left_fork);
			pthread_mutex_lock(p->right_fork);
			printf("%ld %d has taken a fork\n", (t_ms() - rule.start_time)
				/ 1000, p->id);
			printf("%ld %d is eating\n", (t_ms() - rule.start_time) / 1000,
				p->id);
			usleep(rule.time_to_eat);
			p->last_eat_time = t_ms();
			pthread_mutex_unlock(p->right_fork);
			pthread_mutex_unlock(p->left_fork);
			p->meal_count++;
			if (p->meal_count == rule.number_of_times_each_philo_eat)
				return (NULL);
			printf("%ld %d is sleeping\n", (t_ms() - rule.start_time) / 1000,
				p->id);
			usleep(rule.time_to_sleep);
			printf("%ld %d is thinking\n", (t_ms() - rule.start_time) / 1000,
				p->id);
		}
	}
	return (NULL);
}

void	make_thread(t_data *data)
{
	pthread_mutex_t	forks[data->rule_data.number_of_philo];
	int				i;

	i = 0;
	data->rule_data.start_time = t_ms();
	while (i < data->rule_data.number_of_philo)
	{
		pthread_mutex_init(&forks[i], NULL);
		data->philos_data[i].id = i;
		data->philos_data[i].right_fork = &forks[i];
		data->philos_data[i].left_fork = &forks[(i + 1)
			% data->rule_data.number_of_philo];
		data->philos_data[i].data = data;
		data->philos_data[i].last_eat_time = data->rule_data.start_time;
		pthread_create(&data->philos_data[i].thread, NULL, routine,
			&data->philos_data[i]);
		i++;
	}
	pthread_create(&data->thread, NULL, check_death, data);
	pthread_join(data->thread, NULL);
	return ;
	for (i = 0; i < data->rule_data.number_of_philo; i++)
		pthread_join(data->philos_data[i].thread, NULL);
}

void	insert_rule(t_rule *rule, int ac, char **argv)
{
	rule->number_of_philo = ft_atoi(argv[1]);
	rule->time_to_die = ft_atoi(argv[2]) * 1000;
	rule->time_to_eat = ft_atoi(argv[3]) * 1000;
	rule->time_to_sleep = ft_atoi(argv[4]) * 1000;
	if (ac == 5)
		rule->number_of_times_each_philo_eat = ft_atoi(argv[5]);
	else
		rule->number_of_times_each_philo_eat = -1;
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
		return (1);
	printf("start\n");
	insert_rule(&data.rule_data, argc - 1, argv);
	data.philos_data = malloc(sizeof(t_philo) * data.rule_data.number_of_philo);
	if (!data.philos_data)
	{
		perror("Error:");
		return (1);
	}
	make_thread(&data);
	return (0);
}
