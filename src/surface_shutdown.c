// SPDX-License-Identifier: GPL-2.0-or-later

#include <linux/dmi.h>
#include <linux/efi.h>
#include <linux/err.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/reboot.h>

static struct sys_off_handler *surface_sys_off_handler;

static int surface_power_off(struct sys_off_data *data)
{
	efi.reset_system(EFI_RESET_SHUTDOWN, EFI_SUCCESS, 0, NULL);

	return NOTIFY_DONE;
}

static int __init surface_shutdown_init(void)
{
	surface_sys_off_handler = register_sys_off_handler(
		SYS_OFF_MODE_POWER_OFF, SYS_OFF_PRIO_FIRMWARE + 1, surface_power_off, NULL);

	return PTR_ERR_OR_ZERO(surface_sys_off_handler);
}

static void __exit surface_shutdown_exit(void)
{
	if (!surface_sys_off_handler)
		return;

	unregister_sys_off_handler(surface_sys_off_handler);
}

module_init(surface_shutdown_init);
module_exit(surface_shutdown_exit);

static const struct dmi_system_id surface_shutdown_ids[] __initconst = {
	{
		.ident = "Surface Pro 9",
		.matches = {
			DMI_EXACT_MATCH(DMI_SYS_VENDOR, "Microsoft Corporation"),
			DMI_EXACT_MATCH(DMI_PRODUCT_NAME, "Surface Pro 9"),
		},
	},
	{
		.ident = "Surface Laptop 5",
		.matches = {
			DMI_EXACT_MATCH(DMI_SYS_VENDOR, "Microsoft Corporation"),
			DMI_EXACT_MATCH(DMI_PRODUCT_NAME, "Surface Laptop 5"),
		},
	},
	{},
};
MODULE_DEVICE_TABLE(dmi, surface_shutdown_ids);

MODULE_DESCRIPTION("Forces EFI shutdown on Surface devices that cannot power off using ACPI");
MODULE_LICENSE("GPL");
