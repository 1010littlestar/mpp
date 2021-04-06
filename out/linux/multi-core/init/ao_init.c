#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>

#include "hi_type.h"
#include "ao_exp.h"

extern int ao_module_init(void);
extern void ao_module_exit(void);

extern AO_EXPORT_SYMBOL_S g_stAoExpSymbol;
EXPORT_SYMBOL(g_stAoExpSymbol);

static int __init ao_mod_init(void){
    ao_module_init();
    return 0;
}
static void __exit ao_mod_exit(void){
    ao_module_exit();
}

module_init(ao_mod_init);
module_exit(ao_mod_exit);

MODULE_LICENSE("Proprietary");
