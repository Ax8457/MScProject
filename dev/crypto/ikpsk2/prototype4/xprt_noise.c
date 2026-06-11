// xprt_noise.c
#include <linux/module.h>
#include <linux/sunrpc/xprt.h>

#define XPRT_TRANSPORT_NOISE 256

static struct rpc_xprt *xs_noise_setup(struct xprt_create *args)
{
    return ERR_PTR(-ENOSYS);  // placeholder — étape suivante
}

static struct xprt_class xs_noise_transport = {
    .list   = LIST_HEAD_INIT(xs_noise_transport.list),
    .name   = "noise transport",
    .owner  = THIS_MODULE,
    .ident  = XPRT_TRANSPORT_NOISE,
    .setup  = xs_noise_setup,
};

static int __init xprt_noise_init(void)
{
    int ret = xprt_register_transport(&xs_noise_transport);
    if (ret)
        pr_err("xprt_noise: registration failed (%d)\n", ret);
    else
        pr_info("xprt_noise: registered\n");
    return ret;
}

static void __exit xprt_noise_exit(void)
{
    xprt_unregister_transport(&xs_noise_transport);
    pr_info("xprt_noise: unregistered\n");
}

module_init(xprt_noise_init);
module_exit(xprt_noise_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Noise IKpsk2 transport for sunrpc");
MODULE_AUTHOR("Axel Biegalski");
