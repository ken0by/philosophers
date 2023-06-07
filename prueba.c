static int	is_dead(t_philo *philo)
{
	uint64_t	time;

	pthread_mutex_lock(philo->table->times_eat_m);
	time = get_current_time() - philo->last;
	if (time > philo->table->time_to_dead)
	{
		pthread_mutex_unlock(philo->table->times_eat_m);
		pthread_mutex_lock(philo->table->died_m);
		philo->table->flag_dead = 1;
		print_msg_dead(philo);
		pthread_mutex_unlock(philo->table->died_m);
		//pthread_mutex_unlock(philo->table->times_eat_m);
		return (1);
	}
	//pthread_mutex_unlock(philo->table->died_m);
	pthread_mutex_unlock(philo->table->times_eat_m);
	return (0);
}

static int	check_dead(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_philo)
	{
		if (is_dead(table->philosophers[i]))
		{
			pthread_mutex_lock(table->msg);
			table->print = 0;
			pthread_mutex_unlock(table->msg);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	filo_end_eat(t_table *table)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (i < table->n_philo)
	{
		pthread_mutex_lock(table->times_eat_m);
		if (table->philosophers[i]->times_eat >= table->times_must_eat)
			j++;
		pthread_mutex_unlock(table->times_eat_m);
		if (j == table->n_philo)
		{
			printf("All are full\n");
			pthread_mutex_lock(table->end_m);
			table->finish = 1;
			pthread_mutex_unlock(table->end_m);
			return (1);
		}
	}
	return (0);
}

static void	finish_start(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_philo)
	{
		pthread_mutex_unlock(table->philosophers[i]->l_fork);
		pthread_join(table->philo_thread[i], NULL);
		i++;
	}
	i = 0;
	while (i < table->n_philo)
	{
		pthread_mutex_destroy(table->fork + i);
		pthread_mutex_destroy(table->msg + i);
		i++;
	}
	pthread_mutex_destroy(table->start_m);
	pthread_mutex_destroy(table->times_eat_m);
	pthread_mutex_destroy(table->died_m);
	pthread_mutex_destroy(table->end_m);
	free_table(table);
}

int	finish(t_table *table, int argc)
{
	int	i;

	pthread_mutex_lock(table->start_m);
	pthread_mutex_unlock(table->start_m);
	i = 1;
	while (i)
	{
		if (argc == 6)
			if (filo_end_eat(table))
				i = 0;
		if (check_dead(table))
			i = 0;
		pthread_mutex_lock(table->died_m);
		if (table->flag_dead)
			i = 0;
		pthread_mutex_unlock(table->died_m);
	}
	pthread_mutex_lock(table->msg);
	table->print = 0;
	pthread_mutex_unlock(table->msg);
	finish_start(table);
	return (1);
}

int	ft_atoi(const char *str)
{
	int	nb;
	int	x;
	int	k;

	nb = 0;
	x = 0;
	k = 1;
	while ((str[x] == ' ') || (str[x] == '\f') || (str[x] == '\n')
		|| (str[x] == '\r') || (str[x] == '\t') || (str[x] == '\v'))
		x++;
	if (str[x] == '+' || str[x] == '-')
	{
		if (str[x] == '-')
			k = k * (-1);
		x++;
	}
	while (str[x] >= '0' && str[x] <= '9')
	{
		nb = nb * 10 + str[x] - 48;
		x++;
	}
	return (nb * k);
}

void	ft_usleep(uint64_t time)
{
	uint64_t	t;

	t = time + get_current_time();
	while (get_current_time() < t)
		usleep(100);
}

void	free_table(t_table *table)
{
	int	i;

	i = -1;
	while (++i <= table->n_philo)
		free(table->philosophers[i]);
	free(table->fork);
	free(table->msg);
	free(table->start_m);
	free(table->times_eat_m);
	free(table->died_m);
	free(table->end_m);
	free(table->philo_thread);
}

void	free_mtx(t_table *table, int i)
{
	int	j;

	j = 0;
	while (j <= i)
	{
		free(table->philosophers[j]);
		j++;
	}
	free(table->philosophers);
}

uint64_t	get_current_time(void)
{
	struct timeval	tv;
	uint64_t		millisec;

	gettimeofday(&tv, NULL);
	millisec = (uint64_t)tv.tv_sec * 1000
		+ (uint64_t)tv.tv_usec / 1000;
	return (millisec);
}

void	print_msg_dead(t_philo *philo)
{
	uint64_t	time;

	time = get_current_time() - philo->table->start;
	pthread_mutex_lock(philo->msg);
	if (philo->table->print)
	{
		printf("%llu %d", time, philo->id);
		printf("\033[31;3m 💀 DIED 💀 \033[0m\n");
	}
	pthread_mutex_unlock(philo->msg);
}

void	print_msg(t_philo *philo, char *s)
{
	uint64_t	time;

	time = get_current_time() - philo->table->start;
	pthread_mutex_lock(philo->msg);
	if (philo->table->print)
		printf("%llu %d %s", time, philo->id, s);
	pthread_mutex_unlock(philo->msg);
}

static int	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	print_msg(philo, "has take a fork");
	pthread_mutex_lock(philo->r_fork);
	print_msg(philo, "has take a fork");
	pthread_mutex_lock(philo->table->times_eat_m);
	philo->times_eat++;
	philo->last = get_current_time();
	pthread_mutex_unlock(philo->table->times_eat_m);
	print_msg(philo, "is eating");
	ft_usleep(philo->table->time_to_eat);
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
	return (1);
}

static int	sleep_think(t_philo *philo)
{
	print_msg(philo, "is sleeping");
	ft_usleep(philo->table->time_to_sleep);
	print_msg(philo, "is thinking");
	return (1);
}

static int	actions(t_philo *philo)
{
	if (!eat(philo))
		return (0);
	if (!sleep_think(philo))
		return (0);
	return (1);
}

static void	*filosofofo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->table->start_m);
	pthread_mutex_unlock(philo->table->start_m);
	if ((philo->id % 2) == 0)
		usleep(100);
	pthread_mutex_lock(philo->table->died_m);
	pthread_mutex_lock(philo->table->end_m);
	while (!philo->table->flag_dead && philo->table->finish)
	{
		pthread_mutex_unlock(philo->table->died_m);
		pthread_mutex_unlock(philo->table->end_m);
		if (!actions(philo))
			break ;
		pthread_mutex_lock(philo->table->died_m);
		pthread_mutex_lock(philo->table->end_m);
	}
	pthread_mutex_unlock(philo->table->died_m);
	pthread_mutex_unlock(philo->table->end_m);
	return (NULL);
}

int	start_meal(t_table *table)
{
	int			i;
	t_philo		*philo_s;
	pthread_t	*philo_t;

	table->start_m = malloc(sizeof(pthread_mutex_t));
	if (!table->start_m)
		return (0);
	if (pthread_mutex_init(table->start_m, NULL) != 0)
		return (free(table->start_m), 0);
	pthread_mutex_lock(table->start_m);
	i = -1;
	while (++i < table->n_philo)
	{
		philo_t = table->philo_thread + i;
		philo_s = table->philosophers[i];
		if (pthread_create(philo_t, NULL, filosofofo, philo_s) != 0)
			return (printf("Failed to create a thread!\n"), 0);
	}
	pthread_mutex_unlock(table->start_m);
	return (1);
}

static int	check_args(int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
		return (0);
	i = 0;
	while (++i < argc)
		if (ft_atoi(argv[i]) <= 0)
			return (0);
	return (1);
}

static int	take_args(t_table *table, char **argv)
{
	table->n_philo = ft_atoi(argv[1]);
	table->time_to_dead = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		table->times_must_eat = ft_atoi(argv[5]);
	if (!table->n_philo || !table->time_to_dead
		|| !table->time_to_eat || !table->time_to_sleep
		|| (argv[5] && (!table->times_must_eat)))
		return (0);
	return (1);
}

static int	init_threads(t_table *table)
{
	int	i;

	table->start = get_current_time();
	table->finish = 0;
	table->flag_dead = 0;
	table->print = 1;
	table->philosophers = malloc(sizeof(t_philo *) * table->n_philo);
	i = -1;
	while (++i <= table->n_philo)
	{
		table->philosophers[i] = malloc(sizeof(t_philo));
		if (!table->philosophers[i])
			return (free_mtx(table, i), 0);
	}
	table->msg = malloc(sizeof(pthread_mutex_t));
	if (!table->msg)
		return (free_mtx(table, i), 0);
	if (pthread_mutex_init(table->msg, NULL) != 0)
		return (free_mtx(table, i), free(table->msg), 0);
	table->fork = malloc(sizeof(pthread_mutex_t) * table->n_philo);
	if (!table->fork)
		return (free_mtx(table, i), free(table->msg), 0);
	return (1);
}

static int	init_philo(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < table->n_philo)
	{
		if (pthread_mutex_init(table->fork + i, NULL) != 0)
			return (0);
		philo = table->philosophers[i];
		philo->id = i + 1;
		philo->msg = table->msg;
		philo->l_fork = &(table->fork[(i + 1) % table->n_philo]);
		philo->r_fork = &(table->fork[i]);
		philo->last = table->start;
		philo->table = table;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_table	table;

	if (!check_args(argc, argv))
		return (printf("Invalis arguments\n"), 0);
	if (!take_args(&table, argv))
		return (printf("Error will taking args\n"), 0);
	if (!init_threads(&table))
		return (printf("Failed to initialize threads struct\n"), 0);
	if (!init_philo(&table))
		return (free_mtx(&table, table.n_philo),
			printf("Failed to initialize philosophers struct\n"), 0);
	if (!start_meal(&table))
		return (printf("Failed to start\n"), 0);
	if (!finish(&table, argc))
		return (0);
	printf("OK\n");
	return (0);
}
