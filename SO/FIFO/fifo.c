#define FIFO_PATH "/FIFO/orchestrator_fifo"

void initializefifo() {
   mkfifo(FIFO_PATH, 0666);
   if(mkfifo(FIFO_PATH, 066) == -1){
      if(errno != EEXIST) {
         perror("Erro criado fifo\n");
      }

   }
   
}