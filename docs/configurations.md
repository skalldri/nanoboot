
# Nanoboot memory configurations
Nanoboot supports a number of memory configurations, each with it's own set of pros and cons.

# Config Options

## NO_1BOOT
This option removes 1BOOT. 1BOOT is required to enable 2BOOT updates, so 2BOOT is not updateable with this option enabled.

## SINGLE\_2BOOT\_SLOT
This option 2BOOT dual banking. If you take a bad 2BOOT update, 1BOOT will not be able to recover automatically, and the device will be bricked. 

This option is implied with NO_1BOOT.

## READ\_ONLY\_2BOOT\_SLOT
This option marks the second 2BOOT slot as read only. It must be programmed at the factory with another instance of 2BOOT.

If this option is not set, 1BOOT will overwrite the oldest slot when writing a 2BOOT update.

This option has no effect with NO_1BOOT

## SINGLE\_APP\_SLOT
This option disables app dual banking. The 2BOOT button-combo-rollback functionality will be disabled. If you take a bad App update, you will need to use 2BOOT connected to a host PC to download replacement firmware.

## READ\_ONLY\_APP\_SLOT
This option marks the second App slot as read only. It must be programmed at the factory with another instance of the App.

If this option is not set, 2BOOT will overwrite the oldest slot when writing an App update.

This option has no effect with SINGLE\_APP\_SLOT

# Example Configs

This section contains a number of example nanoboot configurations, ranging from extremly small memory footprint to extremely large. This is not an exhaustive list of memory configurations. As an example, the "Tiny Config" could be changed to include a read-only App slot that is programmed at the factory, allowing a "factory reset" button-combo.

## Tiny Config

Options: NO\_1BOOT, SINGLE\_APP\_SLOT

`
[2BOOT]
[APP]
[SCRATCH]
`

- In this config, 1BOOT is removed
    - This means that 2BOOT is not updateable
    - All the functionalityu of 2BOOT is present, so image verification and firmware updates can be performed for the app
- The scratch space can be larger than the app, but it must be at least as large as the app
- There is no app redundancy: if you take a bad firmware update to the app, 2BOOT cannot automatically roll back to good firmware (it must be downloaded again)

## Small Config

Options: SINGLE\_2BOOT\_SLOT, SINGLE\_APP\_SLOT

`
[1BOOT]
[2BOOT]
[APP]
[SCRATCH]
`

- Similar to Tiny config, but allows 2BOOT to be updated
- 2BOOT updates are not dual-banked
    - This is a risky config: flashing a bad 2BOOT image will result in a bricked device (since 1BOOT cannot rollback)

## 2BOOT Recovery Config

Options: SINGLE\_APP\_SLOT, ( READ\_ONLY\_2BOOT\_SLOT )

`
[1BOOT]
[2BOOT]
[2BOOT (RW / RO)]
[APP]
[SCRATCH]
`

- This config introduces a second 2BOOT slot, which can be configured in two ways:
    - Second slot is read-write: 1BOOT will shuffle 2BOOT images between the two slots to ensure that the oldest 2BOOT is overwritten during 2BOOT updates
    - Second slot is read-only: contains a backup of the factory 2BOOT instance
- Can configure a button-combo which, if held during boot, will restore other 2BOOT slot
- 1BOOT can automatically restore the other 2BOOT slot in the event of repeated boot failure after an update
    - App is responsible for calling a Nanoboot helper routine once it has booted to facilitate this

## Big Config

Options: ( READ\_ONLY\_2BOOT\_SLOT ) ( READ\_ONLY\_APP\_SLOT )

`
[1BOOT]
[2BOOT]
[2BOOT (RW / RO)]
[APP]
[APP (RW / RO)]
[SCRATCH]
`

- This config uses the most flash
- Allows recovery of 2BOOT after a bad update
    - Automatic, or via button-combo during boot
- Allows recovery of App after a bad update
    - Automatic, or via button-combo during boot