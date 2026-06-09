#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

SYMBOL_CRC(g_bridge, 0x6947bd0d, "");

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xe3769724, "kthread_create_on_node" },
	{ 0xb686855b, "wake_up_process" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x89e1659, "kthread_stop" },
	{ 0xc80d0404, "kmalloc_caches" },
	{ 0x52dd86bf, "kmalloc_trace" },
	{ 0x37a0cba, "kfree" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0x7e6fdbfc, "curve25519_base_point" },
	{ 0xc832c670, "curve25519_arch" },
	{ 0x4a5a8811, "curve25519_null_point" },
	{ 0x5a44f8cb, "__crypto_memneq" },
	{ 0x92997ed8, "_printk" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0x6bd0e573, "down_interruptible" },
	{ 0xe0a683ed, "current_task" },
	{ 0x8ddd8aad, "schedule_timeout" },
	{ 0xe1a29ec0, "handshake_consume_initiation" },
	{ 0x4d02bf, "handshake_create_response" },
	{ 0xcf2a6966, "up" },
	{ 0x39c12ac2, "begin_session" },
	{ 0x167c5967, "print_hex_dump" },
	{ 0x160c03af, "module_layout" },
};

MODULE_INFO(depends, "libcurve25519-generic,curve25519-x86_64,ikpsk2_crypto");

