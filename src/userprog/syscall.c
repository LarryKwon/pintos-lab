#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include <devices/shutdown.h>
#include <filesys/filesys.h>

static void syscall_handler(struct intr_frame *);

void syscall_init(void)
{
  intr_register_int(0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler(struct intr_frame *f UNUSED)
{
  // printf("system call!\n");
  int32_t args[4];
  check_address(f->esp);
  int32_t *esp = (int32_t *)f->esp+1;
  switch(*(int32_t *)f->esp){
    case SYS_HALT:
      halt();
      break;

    case SYS_EXIT:
      get_argument(esp, args,1);
      exit(args[0]);
      break;

    case SYS_CREATE:
      get_argument(esp, args,2);
      f->eax = create(args[0],args[1]);
      break;

    case SYS_REMOVE:
      get_argument(esp,args,1);
      f->eax = remove(args[0]);
      break;
  }

  thread_exit();
}

void check_address(void *addr)
{
  uint32_t addr_val = *(uint32_t *)addr;
  if (!is_user_vaddr(addr) || addr_val < 0x08048000)
    exit(-1);
}

void get_argument(void *esp, int *arg, int count)
{
  ASSERT(count <= 4 && count >= 1);
  int i;
  for (i = 0; i < count; i++)
  {
    check_address(esp);
    check_address(esp+4);
    (*arg) = *(int32_t *)esp;
    arg++;
    (int32_t *)esp++;
  }
}

void halt(void){
  shutdown_power_off();
}

void exit(int status){
  thread_exit();
}

bool create(const char *file, unsigned initial_size){
  return filesys_create(file, initial_size);
}

bool remove(const char *file){
  return filesys_remove(file);
}

/* Reads a byte at user virtual address UADDR.
UADDR must be below PHYS_BASE.
Returns the byte value if successful, -1 if a segfault
occurred. */
static int
get_user(const uint8_t *uaddr)
{

  int result;
  asm("movl $1f, %0; movzbl %1, %0; 1:"
      : "=&a"(result)
      : "m"(*uaddr));
  return result;
}
/* Writes BYTE to user address UDST.
UDST must be below PHYS_BASE.
Returns true if successful, false if a segfault occurred. */
static bool
put_user(uint8_t *udst, uint8_t byte)
{
  int error_code;
  asm("movl $1f, %0; movb %b2, %1; 1:"
      : "=&a"(error_code), "=m"(*udst)
      : "q"(byte));
  return error_code != -1;
}