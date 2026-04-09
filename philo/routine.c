#include "philo.h"

int is_dead(t_data *data)
{
	int flag;

	pthread_mutex_lock(&data->death_flag_mtx);
	flag = data->death_flag;
	pthread_mutex_unlock(&data->death_flag_mtx);
	return (flag);
}

int is_all_ate(t_data *data)
{
	int count;

	pthread_mutex_lock(&data->all_ate_mtx);
	count = data->all_ate_count;
	pthread_mutex_unlock(&data->all_ate_mtx);
	return (count == data->rule_data.number_of_philo);
}

void *check_death(void *arg)
{
	t_data *data;

	data = (t_data *)arg;
	while (!is_all_ate(data))
	{
		for (int i = 0; i < data->rule_data.number_of_philo; i++)
		{
			pthread_mutex_lock(&data->philos_data[i].eat_time);
			if (t_ms() - data->philos_data[i].last_eat_time > data->rule_data.time_to_die)
			{
				pthread_mutex_unlock(&data->philos_data[i].eat_time);
				pthread_mutex_lock(&data->death_flag_mtx);
				data->death_flag = 1;
				pthread_mutex_unlock(&data->death_flag_mtx);
				printf("%ld %d died\n", t_ms() - data->rule_data.start_time,
				data->philos_data[i].id);
				return (NULL);
			}
			pthread_mutex_unlock(&data->philos_data[i].eat_time);
		}
		usleep(100);
	}
	return (NULL);
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
			if (is_dead(data))
			{
				pthread_mutex_unlock(p->right_fork);
				return (NULL);
			}
			printf("%ld %d has taken a fork\n", t_ms() - rule.start_time,
				   p->id);
			pthread_mutex_lock(p->left_fork);
			if (is_dead(data))
			{
				pthread_mutex_unlock(p->right_fork);
				pthread_mutex_unlock(p->left_fork);
				return (NULL);
			}
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
			{
				pthread_mutex_lock(&data->all_ate_mtx);
				data->all_ate_count++;
				pthread_mutex_unlock(&data->all_ate_mtx);
				return (NULL);
			}
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
			if (is_dead(data))
			{
				pthread_mutex_unlock(p->left_fork);
				return (NULL);
			}
			printf("%ld %d has taken a fork\n", t_ms() - rule.start_time,
				   p->id);
			pthread_mutex_lock(p->right_fork);
			if (is_dead(data))
			{
				pthread_mutex_unlock(p->left_fork);
				pthread_mutex_unlock(p->right_fork);
				return (NULL);
			}
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
			{
				pthread_mutex_lock(&data->all_ate_mtx);
				data->all_ate_count++;
				pthread_mutex_unlock(&data->all_ate_mtx);
				return (NULL);
			}
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
