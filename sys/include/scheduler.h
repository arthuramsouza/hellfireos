void dispatch_isr(void *arg);
int32_t sched_lottery(void);
int32_t sched_priorityrr(void);
int32_t sched_rma(void);
int32_t sched_aperiodic(void);
void add_to_delay_queue(delay_node** node_aux);
void print_jitter();
