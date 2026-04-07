/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knomura <knomura@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 16:45:08 by knomura           #+#    #+#             */
/*   Updated: 2026/04/07 18:59:05 by knomura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *check_death(void *arg)
{
	t_data *data;

	data = (t_data *)arg;
	while (1)
	{
		for (int i = 0; i < data->rule_data.number_of_philo; i++)
		{
			pthread_mutex_lock(&data->philos_data[i].eat_time);
			if (t_ms() - data->philos_data[i].last_eat_time > data->rule_data.time_to_die)
			{
				printf("%ld %d died\n", t_ms() - data->rule_data.start_time,
					   data->philos_data[i].id);
				pthread_mutex_unlock(&data->philos_data[i].eat_time);
				pthread_mutex_lock(&data->death_flag_mtx);
				data->death_flag = 1;
				pthread_mutex_unlock(&data->death_flag_mtx);
				return (NULL);
			}
			pthread_mutex_unlock(&data->philos_data[i].eat_time);
		}
		usleep(100);
	}
}

int is_dead(t_data *data)
{
	int flag;

	pthread_mutex_lock(&data->death_flag_mtx);
	flag = data->death_flag;
	pthread_mutex_unlock(&data->death_flag_mtx);
	return (flag);
}

void *routine(void *arg)
{
	t_philo *p;
	t_data *data;
	t_rule rule;

	p = (t_philo *)arg;
	data = p->data;
	rule = data->rule_data;
	while (!is_dead(data))
	{
		if (p->id % 2 == 0)
		{
			pthread_mutex_lock(p->right_fork);
			printf("%ld %d has taken a fork\n", t_ms() - rule.start_time,
				   p->id);
			pthread_mutex_lock(p->left_fork);
			printf("%ld %d has taken a fork\n", t_ms() - rule.start_time,
				   p->id);
			printf("%ld %d is eating\n", t_ms() - rule.start_time, p->id);
			pthread_mutex_lock(&p->eat_time);
			p->last_eat_time = t_ms();
			pthread_mutex_unlock(&p->eat_time);
			usleep(rule.time_to_eat * 1000);
			pthread_mutex_lock(&p->eat_time);
			p->last_eat_time = t_ms();
			pthread_mutex_unlock(&p->eat_time);
			pthread_mutex_unlock(p->right_fork);
			pthread_mutex_unlock(p->left_fork);
			p->meal_count++;
			if (p->meal_count == rule.number_of_times_each_philo_eat)
				return (NULL);
			if (is_dead(data))
				return (NULL);
			printf("%ld %d is sleeping\n", t_ms() - rule.start_time, p->id);
			usleep(rule.time_to_sleep * 1000);
			if (is_dead(data))
				return (NULL);
			printf("%ld %d is thinking\n", t_ms() - rule.start_time, p->id);
		}
		else
		{
			pthread_mutex_lock(p->left_fork);
			printf("%ld %d has taken a fork\n", t_ms() - rule.start_time,
				   p->id);
			pthread_mutex_lock(p->right_fork);
			printf("%ld %d has taken a fork\n", t_ms() - rule.start_time,
				   p->id);
			printf("%ld %d is eating\n", t_ms() - rule.start_time, p->id);
			pthread_mutex_lock(&p->eat_time);
			p->last_eat_time = t_ms();
			pthread_mutex_unlock(&p->eat_time);
			usleep(rule.time_to_eat * 1000);
			pthread_mutex_lock(&p->eat_time);
			p->last_eat_time = t_ms();
			pthread_mutex_unlock(&p->eat_time);
			pthread_mutex_unlock(p->left_fork);
			pthread_mutex_unlock(p->right_fork);
			p->meal_count++;
			if (p->meal_count == rule.number_of_times_each_philo_eat)
				return (NULL);
			if (is_dead(data))
				return (NULL);
			printf("%ld %d is sleeping\n", t_ms() - rule.start_time, p->id);
			usleep(rule.time_to_sleep * 1000);
			if (is_dead(data))
				return (NULL);
			printf("%ld %d is thinking\n", t_ms() - rule.start_time, p->id);
		}
	}
	return (NULL);
}

void destroy_forks_and_exit(t_data *data, int count, int status)
{
	int i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	free(data->forks);
	free(data->philos_data);
	exit(status);
}

void make_thread(t_data *data)
{
	int i;
	int ret;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->rule_data.number_of_philo);
	if (!data->forks)
		exit(1);
	i = 0;
	data->rule_data.start_time = t_ms();
	while (i < data->rule_data.number_of_philo)
	{
		ret = pthread_mutex_init(&data->forks[i], NULL);
		if (ret)
			destroy_forks_and_exit(data, i, ret);
		data->philos_data[i].id = i + 1;
		data->philos_data[i].right_fork = &data->forks[i];
		data->philos_data[i].left_fork = &data->forks[(i + 1) % data->rule_data.number_of_philo];
		data->philos_data[i].data = data;
		data->philos_data[i].last_eat_time = data->rule_data.start_time;
		ret = pthread_mutex_init(&data->philos_data[i].eat_time, NULL);
		if (ret)
			destroy_forks_and_exit(data, i + 1, ret);
		ret = pthread_create(&data->philos_data[i].thread, NULL, routine,
							 &data->philos_data[i]);
		if (ret)
			destroy_forks_and_exit(data, i + 1, ret);
		i++;
	}
	pthread_create(&data->thread, NULL, check_death, data);
	pthread_join(data->thread, NULL);
	// return ;
	for (i = 0; i < data->rule_data.number_of_philo; i++)
		pthread_join(data->philos_data[i].thread, NULL);
}

void insert_rule(t_rule *rule, int ac, char **argv)
{
	rule->number_of_philo = ft_atoi(argv[1]);
	rule->time_to_die = ft_atoi(argv[2]);
	rule->time_to_eat = ft_atoi(argv[3]);
	rule->time_to_sleep = ft_atoi(argv[4]);
	if (ac == 6)
		rule->number_of_times_each_philo_eat = ft_atoi(argv[5]);
	else
		rule->number_of_times_each_philo_eat = -1;
}

int main(int argc, char **argv)
{
	t_data data;
	int ret;

	if (argc != 5 && argc != 6)
		return (1);
	insert_rule(&data.rule_data, argc, argv);
	ret = pthread_mutex_init(&data.death_flag_mtx, NULL);
	if (ret)
	{
		write(2, "Error: mutex init failed\n", 25);
		return (ret);
	}
	data.death_flag = 0;
	data.philos_data = malloc(sizeof(t_philo) * data.rule_data.number_of_philo);
	if (!data.philos_data)
	{
		write(2, "Error: malloc failed\n", 21);
		return (1);
	}
	make_thread(&data);
	return (0);
}
