BLE 5.1 support for 5.0 nRF hardware
************************************

This fork of nRF Connect's sdk-zephyr repository adds support for BLE 5.1
connectionless Angle of Arrival (AoA) RTLS beacons on non-5.1 compliant nRF hardware.

This is achieved by extending the Nordic BLE stack LLL's Periodic Advertising Extensions
capability to include the addition of a Constant Tone Extension (CTE).

This CTE capability is implemented purely in software and therefore
does not require 5.1-compliant hardware such as the nRF52833 or nRF5340.

It can run on BLE 5.0 hardware such as the nRF52840, and has been tested on
the nRF52840 PCA10059 dongle.

Add ``BT_CTLR_DF_SOFTCTE`` as a Kconfig flag to enable.

This repository also contains a minimal beacon implementation that does not
use the bluetooth stack, but rather only uses the nRF Radio, Clock and Timer
peripherals. It transmits a simple pre-5.0 BLE packet which includes TX Power,
and also transmits a CTE. Both of these can be used to test positioning and
RTLS systems without the need for 5.1-compliant hardware.

.. raw:: html

   <a href="https://www.zephyrproject.org">
     <p align="center">
       <img src="doc/images/Zephyr-Project.png">
     </p>
   </a>

   <a href="https://bestpractices.coreinfrastructure.org/projects/74"><img
   src="https://bestpractices.coreinfrastructure.org/projects/74/badge"></a>
   <a href="https://buildkite.com/zephyr/zephyr">
   <img
   src="https://badge.buildkite.com/f5bd0dc88306cee17c9b38e78d11bb74a6291e3f40e7d13f31.svg?branch=master"></a>


The Zephyr Project is a scalable real-time operating system (RTOS) supporting
multiple hardware architectures, optimized for resource constrained devices,
and built with security in mind.

The Zephyr OS is based on a small-footprint kernel designed for use on
resource-constrained systems: from simple embedded environmental sensors and
LED wearables to sophisticated smart watches and IoT wireless gateways.

The Zephyr kernel supports multiple architectures, including ARM Cortex-M,
Intel x86, ARC, Nios II, Tensilica Xtensa, and RISC-V, and a large number of
`supported boards`_.

.. below included in doc/introduction/introduction.rst

.. start_include_here

Getting Started
***************

Welcome to Zephyr! See the `Introduction to Zephyr`_ for a high-level overview,
and the documentation's `Getting Started Guide`_ to start developing.

Community Support
*****************

Community support is provided via mailing lists and Slack; see the Resources
below for details.

.. _project-resources:

Resources
*********

Here's a quick summary of resources to help you find your way around:

* **Help**: `Asking for Help Tips`_
* **Documentation**: http://docs.zephyrproject.org (`Getting Started Guide`_)
* **Source Code**: https://github.com/zephyrproject-rtos/zephyr is the main
  repository; https://elixir.bootlin.com/zephyr/latest/source contains a
  searchable index
* **Releases**: https://github.com/zephyrproject-rtos/zephyr/releases
* **Samples and example code**: see `Sample and Demo Code Examples`_
* **Mailing Lists**: users@lists.zephyrproject.org and
  devel@lists.zephyrproject.org are the main user and developer mailing lists,
  respectively. You can join the developer's list and search its archives at
  `Zephyr Development mailing list`_. The other `Zephyr mailing list
  subgroups`_ have their own archives and sign-up pages.
* **Nightly CI Build Status**: https://lists.zephyrproject.org/g/builds
  The builds@lists.zephyrproject.org mailing list archives the CI
  (shippable) nightly build results.
* **Chat**: Zephyr's Slack workspace is https://zephyrproject.slack.com.  Use
  this `Slack Invite`_ to register.
* **Contributing**: see the `Contribution Guide`_
* **Wiki**: `Zephyr GitHub wiki`_
* **Issues**: https://github.com/zephyrproject-rtos/zephyr/issues
* **Security Issues**: Email vulnerabilities@zephyrproject.org to report
  security issues; also see our `Security`_ documentation. Security issues are
  tracked separately at https://zephyrprojectsec.atlassian.net.
* **Zephyr Project Website**: https://zephyrproject.org

.. _Slack Invite: https://tinyurl.com/y5glwylp
.. _supported boards: http://docs.zephyrproject.org/latest/boards/index.html
.. _Zephyr Documentation: http://docs.zephyrproject.org
.. _Introduction to Zephyr: http://docs.zephyrproject.org/latest/introduction/index.html
.. _Getting Started Guide: http://docs.zephyrproject.org/latest/getting_started/index.html
.. _Contribution Guide: http://docs.zephyrproject.org/latest/contribute/index.html
.. _Zephyr GitHub wiki: https://github.com/zephyrproject-rtos/zephyr/wiki
.. _Zephyr Development mailing list: https://lists.zephyrproject.org/g/devel
.. _Zephyr mailing list subgroups: https://lists.zephyrproject.org/g/main/subgroups
.. _Sample and Demo Code Examples: http://docs.zephyrproject.org/latest/samples/index.html
.. _Security: http://docs.zephyrproject.org/latest/security/index.html
.. _Asking for Help Tips: https://docs.zephyrproject.org/latest/guides/getting-help.html
