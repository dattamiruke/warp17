/*
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 *
 * Copyright (c) 2016, Juniper Networks, Inc. All rights reserved.
 *
 *
 * The contents of this file are subject to the terms of the BSD 3 clause
 * License (the "License"). You may not use this file except in compliance
 * with the License.
 *
 * You can obtain a copy of the license at
 * https://github.com/Juniper/warp17/blob/master/LICENSE.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * File name:
 *     tpg_test_mgmt_cli.c
 *
 * Description:
 *     Test manager CLI.
 *
 * Author:
 *     Dumitru Ceara, Eelco Chaudron
 *
 * Initial Created:
 *     22/12/2015
 *
 * Notes:
 *
 */

/*****************************************************************************
 * Include files
 ****************************************************************************/
#include "tcp_generator.h"

/****************************************************************************
 * - "start/stop tests "
 ****************************************************************************/
struct cmd_tests_start_stop_result {
    cmdline_fixed_string_t  start;
    cmdline_fixed_string_t  stop;
    cmdline_fixed_string_t  tests;
    cmdline_fixed_string_t  port_kw;
    uint32_t                port;
};

static cmdline_parse_token_string_t cmd_tests_start_T_start =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_start_stop_result, start, "start");
static cmdline_parse_token_string_t cmd_tests_start_T_stop =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_start_stop_result, stop, "stop");
static cmdline_parse_token_string_t cmd_tests_start_T_tests =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_start_stop_result, tests, "tests");
static cmdline_parse_token_string_t cmd_tests_start_T_port_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_start_stop_result, port_kw, "port");
static cmdline_parse_token_num_t cmd_tests_start_T_port =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_start_stop_result, port, UINT32);

static void cmd_tests_start_parsed(void *parsed_result,
                                   struct cmdline *cl,
                                   void *data __rte_unused)
{
    printer_arg_t                       parg;
    struct cmd_tests_start_stop_result *pr;
    struct rte_eth_link                 link;

    parg = TPG_PRINTER_ARG(cli_printer, cl);
    pr = parsed_result;

    /*
     * Use the nowait link call as rte_eth_link_get might block for 9 seconds.
     * Might be nice though if the user could specify a timeout and/or retry
     * count.
     */
    rte_eth_link_get_nowait(pr->port, &link);
    if (!link.link_status) {
        cmdline_printf(cl, "ERROR: Ehernet port %u: Link down!\n", pr->port);
        return;
    }

    if (test_mgmt_start_port(pr->port, &parg) == 0)
        cmdline_printf(cl, "Tests started on port %"PRIu32"\n", pr->port);
    else
        cmdline_printf(cl, "ERROR: Failed to start tests on port %"PRIu32"!\n",
                       pr->port);
}

static void cmd_tests_stop_parsed(void *parsed_result __rte_unused,
                                  struct cmdline *cl __rte_unused,
                                  void *data __rte_unused)
{
    printer_arg_t                       parg;
    struct cmd_tests_start_stop_result *pr;

    parg = TPG_PRINTER_ARG(cli_printer, cl);
    pr = parsed_result;

    if (test_mgmt_stop_port(pr->port, &parg) == 0)
        cmdline_printf(cl, "Tests stopped on port %"PRIu32"\n", pr->port);
    else
        cmdline_printf(cl, "ERROR: Failed to stop tests on port %"PRIu32"!\n",
                       pr->port);
}

cmdline_parse_inst_t cmd_tests_start = {
    .f = cmd_tests_start_parsed,
    .data = NULL,
    .help_str = "start tests port <port>",
    .tokens = {
        (void *)&cmd_tests_start_T_start,
        (void *)&cmd_tests_start_T_tests,
        (void *)&cmd_tests_start_T_port_kw,
        (void *)&cmd_tests_start_T_port,
        NULL,
    },
};

cmdline_parse_inst_t cmd_tests_stop = {
    .f = cmd_tests_stop_parsed,
    .data = NULL,
    .help_str = "stop tests port <port>",
    .tokens = {
        (void *)&cmd_tests_start_T_stop,
        (void *)&cmd_tests_start_T_tests,
        (void *)&cmd_tests_start_T_port_kw,
        (void *)&cmd_tests_start_T_port,
        NULL,
    },
};

/****************************************************************************
 * - "show tests ui"
 ****************************************************************************/
struct cmd_show_tests_ui_result {
    cmdline_fixed_string_t show;
    cmdline_fixed_string_t tests;
    cmdline_fixed_string_t ui;
};

static cmdline_parse_token_string_t cmd_show_tests_ui_T_show =
    TOKEN_STRING_INITIALIZER(struct cmd_show_tests_ui_result, show, "show");
static cmdline_parse_token_string_t cmd_show_tests_ui_T_tests =
    TOKEN_STRING_INITIALIZER(struct cmd_show_tests_ui_result, tests, "tests");
static cmdline_parse_token_string_t cmd_show_tests_ui_T_ui =
    TOKEN_STRING_INITIALIZER(struct cmd_show_tests_ui_result, ui, "ui");

static void cmd_show_tests_ui_parsed(void *parsed_result __rte_unused,
                                        struct cmdline *cl __rte_unused,
                                        void *data __rte_unused)
{
    test_init_stats_screen();
}

cmdline_parse_inst_t cmd_show_tests_ui = {
    .f = cmd_show_tests_ui_parsed,
    .data = NULL,
    .help_str = "show tests ui",
    .tokens = {
        (void *)&cmd_show_tests_ui_T_show,
        (void *)&cmd_show_tests_ui_T_tests,
        (void *)&cmd_show_tests_ui_T_ui,
        NULL,
    },
};

/****************************************************************************
 * - "show tests config"
 ****************************************************************************/
struct cmd_show_tests_config_result {
    cmdline_fixed_string_t  show;
    cmdline_fixed_string_t  tests;
    cmdline_fixed_string_t  config;
    cmdline_fixed_string_t  port_kw;
    uint32_t                port;
};

static cmdline_parse_token_string_t cmd_show_tests_config_T_show =
    TOKEN_STRING_INITIALIZER(struct cmd_show_tests_config_result, show, "show");
static cmdline_parse_token_string_t cmd_show_tests_config_T_tests =
    TOKEN_STRING_INITIALIZER(struct cmd_show_tests_config_result, tests, "tests");
static cmdline_parse_token_string_t cmd_show_tests_config_T_config =
    TOKEN_STRING_INITIALIZER(struct cmd_show_tests_config_result, config, "config");
static cmdline_parse_token_string_t cmd_show_tests_config_T_port_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_show_tests_config_result, port_kw, "port");
static cmdline_parse_token_num_t cmd_show_tests_config_T_port =
    TOKEN_NUM_INITIALIZER(struct cmd_show_tests_config_result, port, UINT32);

static void cmd_show_tests_config_parsed(void *parsed_result, struct cmdline *cl,
                                         void *data __rte_unused)
{
    printer_arg_t                        parg;
    struct cmd_show_tests_config_result *pr;
    uint32_t                             tcid;

    parg = TPG_PRINTER_ARG(cli_printer, cl);
    pr = parsed_result;

    if (pr->port >= rte_eth_dev_count()) {
        cmdline_printf(cl, "ERROR: Port should be in the range 0..%"PRIu32"\n",
                       rte_eth_dev_count());
        return;
    }

    test_config_show_port(pr->port, &parg);

    for (tcid = 0; tcid < TPG_TEST_MAX_ENTRIES; tcid++) {
        tpg_test_case_t entry;

        if (test_mgmt_get_test_case_cfg(pr->port, tcid, &entry, NULL) != 0)
            continue;

        cmdline_printf(cl, "Test Case Id: %"PRIu32"\n", tcid);
        test_config_show_tc(&entry, &parg);
        cmdline_printf(cl, "\n\n");
    }
}

cmdline_parse_inst_t cmd_show_tests_config = {
    .f = cmd_show_tests_config_parsed,
    .data = NULL,
    .help_str = "show tests config port <port>",
    .tokens = {
        (void *)&cmd_show_tests_config_T_show,
        (void *)&cmd_show_tests_config_T_tests,
        (void *)&cmd_show_tests_config_T_config,
        (void *)&cmd_show_tests_config_T_port_kw,
        (void *)&cmd_show_tests_config_T_port,
        NULL,
    },
};

/****************************************************************************
 * - "show tests state"
 ****************************************************************************/
struct cmd_show_tests_state_result {
    cmdline_fixed_string_t show;
    cmdline_fixed_string_t tests;
    cmdline_fixed_string_t state;
    cmdline_fixed_string_t port_kw;
    uint32_t               port;
};

static cmdline_parse_token_string_t cmd_show_tests_state_T_show =
    TOKEN_STRING_INITIALIZER(struct cmd_show_tests_state_result, show, "show");
static cmdline_parse_token_string_t cmd_show_tests_state_T_tests =
    TOKEN_STRING_INITIALIZER(struct cmd_show_tests_state_result, tests, "tests");
static cmdline_parse_token_string_t cmd_show_tests_state_T_config =
    TOKEN_STRING_INITIALIZER(struct cmd_show_tests_state_result, state, "state");
static cmdline_parse_token_string_t cmd_show_tests_state_T_port_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_show_tests_state_result, port_kw, "port");
static cmdline_parse_token_num_t cmd_show_tests_state_T_port =
    TOKEN_NUM_INITIALIZER(struct cmd_show_tests_state_result, port, UINT32);

static void cmd_show_tests_state_parsed(void *parsed_result, struct cmdline *cl,
                                        void *data __rte_unused)
{
    printer_arg_t                       parg;
    struct cmd_show_tests_state_result *pr;

    parg = TPG_PRINTER_ARG(cli_printer, cl);
    pr = parsed_result;

    if (pr->port >= rte_eth_dev_count()) {
        cmdline_printf(cl, "ERROR: Port should be in the range 0..%"PRIu32"\n",
                       rte_eth_dev_count());
        return;
    }

    test_state_show_tcs_hdr(pr->port, &parg);
    test_state_show_tcs(pr->port, &parg);
}

cmdline_parse_inst_t cmd_show_tests_state = {
    .f = cmd_show_tests_state_parsed,
    .data = NULL,
    .help_str = "show tests state port <port>",
    .tokens = {
        (void *)&cmd_show_tests_state_T_show,
        (void *)&cmd_show_tests_state_T_tests,
        (void *)&cmd_show_tests_state_T_config,
        (void *)&cmd_show_tests_state_T_port_kw,
        (void *)&cmd_show_tests_state_T_port,
        NULL,
    },
};

/****************************************************************************
 * - "show tests stats"
 ****************************************************************************/
struct cmd_show_tests_stats_result {
    cmdline_fixed_string_t show;
    cmdline_fixed_string_t tests;
    cmdline_fixed_string_t stats;
    cmdline_fixed_string_t port_kw;
    uint32_t               port;
    cmdline_fixed_string_t tcid_kw;
    uint32_t               tcid;
};

static cmdline_parse_token_string_t cmd_show_tests_stats_T_show =
    TOKEN_STRING_INITIALIZER(struct cmd_show_tests_stats_result, show, "show");
static cmdline_parse_token_string_t cmd_show_tests_stats_T_tests =
    TOKEN_STRING_INITIALIZER(struct cmd_show_tests_stats_result, tests,
                             "tests");
static cmdline_parse_token_string_t cmd_show_tests_stats_T_config =
    TOKEN_STRING_INITIALIZER(struct cmd_show_tests_stats_result, stats,
                             "stats");
static cmdline_parse_token_string_t cmd_show_tests_stats_T_port_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_show_tests_stats_result, port_kw,
                             "port");
static cmdline_parse_token_num_t cmd_show_tests_stats_T_port =
    TOKEN_NUM_INITIALIZER(struct cmd_show_tests_stats_result, port, UINT32);
static cmdline_parse_token_string_t cmd_show_tests_stats_T_tcid_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_show_tests_stats_result, tcid_kw,
                             "test-case-id");
static cmdline_parse_token_num_t cmd_show_tests_stats_T_tcid =
    TOKEN_NUM_INITIALIZER(struct cmd_show_tests_stats_result, tcid, UINT32);

static void cmd_show_tests_stats_parsed(void *parsed_result, struct cmdline *cl,
                                        void *data __rte_unused)
{
    printer_arg_t                       parg;
    struct cmd_show_tests_stats_result *pr;
    tpg_test_case_t                     test_case_cfg;

    parg = TPG_PRINTER_ARG(cli_printer, cl);
    pr = parsed_result;

    if (test_mgmt_get_test_case_cfg(pr->port, pr->tcid, &test_case_cfg,
                                    &parg) != 0)
        return;

    cmdline_printf(cl, "Port %"PRIu32", Test Case %"PRIu32" Statistics:\n",
                   pr->port,
                   pr->tcid);

    test_state_show_stats(&test_case_cfg, &parg);
}

cmdline_parse_inst_t cmd_show_tests_stats = {
    .f = cmd_show_tests_stats_parsed,
    .data = NULL,
    .help_str = "show tests stats port <port> test-case-id <tcid>",
    .tokens = {
        (void *)&cmd_show_tests_stats_T_show,
        (void *)&cmd_show_tests_stats_T_tests,
        (void *)&cmd_show_tests_stats_T_config,
        (void *)&cmd_show_tests_stats_T_port_kw,
        (void *)&cmd_show_tests_stats_T_port,
        (void *)&cmd_show_tests_stats_T_tcid_kw,
        (void *)&cmd_show_tests_stats_T_tcid,
        NULL,
    },
};

/****************************************************************************
 * - "add tests l3_intf port <port> ip <ip> mask <mask> gw <gw>"
 ****************************************************************************/
 struct cmd_tests_add_l3_intf_result {
    cmdline_fixed_string_t add;
    cmdline_fixed_string_t tests;
    cmdline_fixed_string_t l3_intf;
    cmdline_fixed_string_t port_kw;
    uint32_t               port;
    cmdline_fixed_string_t ip_kw;
    cmdline_ipaddr_t       ip;
    cmdline_fixed_string_t mask_kw;
    cmdline_ipaddr_t       mask;
};

static cmdline_parse_token_string_t cmd_tests_add_l3_intf_T_add =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_l3_intf_result, add, "add");
static cmdline_parse_token_string_t cmd_tests_add_l3_intf_T_tests =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_l3_intf_result, tests, "tests");
static cmdline_parse_token_string_t cmd_tests_add_l3_intf_T_l3_intf =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_l3_intf_result, l3_intf, "l3_intf");
static cmdline_parse_token_string_t cmd_tests_add_l3_intf_T_port_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_l3_intf_result, port_kw, "port");
static cmdline_parse_token_num_t cmd_tests_add_l3_intf_T_port =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_add_l3_intf_result, port, UINT32);
static cmdline_parse_token_string_t cmd_tests_add_l3_intf_T_ip_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_l3_intf_result, ip_kw, "ip");
static cmdline_parse_token_ipaddr_t cmd_tests_add_l3_intf_T_ip =
    TOKEN_IPADDR_INITIALIZER(struct cmd_tests_add_l3_intf_result, ip);
static cmdline_parse_token_string_t cmd_tests_add_l3_intf_T_mask_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_l3_intf_result, mask_kw, "mask");
static cmdline_parse_token_ipaddr_t cmd_tests_add_l3_intf_T_mask =
    TOKEN_IPADDR_INITIALIZER(struct cmd_tests_add_l3_intf_result, mask);

static void cmd_tests_add_l3_intf_parsed(void *parsed_result, struct cmdline *cl,
                                         void *data __rte_unused)
{
    printer_arg_t                        parg;
    struct cmd_tests_add_l3_intf_result *pr;
    tpg_l3_intf_t                        l3_intf;

    parg = TPG_PRINTER_ARG(cli_printer, cl);
    pr = parsed_result;

    if (pr->ip.family != AF_INET) {
        cmdline_printf(cl, "ERROR: IPv6 not supported yet!\n");
        return;
    }

    if (pr->ip.family != pr->mask.family) {
        cmdline_printf(cl, "ERROR: Mixing IPv4 and IPv6..\n");
        return;
    }

    l3_intf = (tpg_l3_intf_t){
                .l3i_ip.ip_v4 = rte_be_to_cpu_32(pr->ip.addr.ipv4.s_addr),
                .l3i_mask.ip_v4 = rte_be_to_cpu_32(pr->mask.addr.ipv4.s_addr),
                .l3i_count = 1 /* TODO: count not implemented yet! */
            };

    if (test_mgmt_add_port_cfg_l3_intf(pr->port, &l3_intf, &parg) == 0)
        cmdline_printf(cl, "L3 interface successfully added.\n");
    else
        cmdline_printf(cl, "ERROR: Failed to add L3 interface!\n");
}

cmdline_parse_inst_t cmd_tests_add_l3_intf = {
    .f = cmd_tests_add_l3_intf_parsed,
    .data = NULL,
    .help_str = "add tests l3_intf port <port> ip <ip> mask <mask>",
    .tokens = {
        (void *)&cmd_tests_add_l3_intf_T_add,
        (void *)&cmd_tests_add_l3_intf_T_tests,
        (void *)&cmd_tests_add_l3_intf_T_l3_intf,
        (void *)&cmd_tests_add_l3_intf_T_port_kw,
        (void *)&cmd_tests_add_l3_intf_T_port,
        (void *)&cmd_tests_add_l3_intf_T_ip_kw,
        (void *)&cmd_tests_add_l3_intf_T_ip,
        (void *)&cmd_tests_add_l3_intf_T_mask_kw,
        (void *)&cmd_tests_add_l3_intf_T_mask,
        NULL,
    },
};

/****************************************************************************
 * - "add tests l3_gw port <port> gw <gw>"
 ****************************************************************************/
 struct cmd_tests_add_l3_gw_result {
    cmdline_fixed_string_t add;
    cmdline_fixed_string_t tests;
    cmdline_fixed_string_t l3_gw;
    cmdline_fixed_string_t port_kw;
    uint32_t               port;
    cmdline_fixed_string_t gw_kw;
    cmdline_ipaddr_t       gw;
};

static cmdline_parse_token_string_t cmd_tests_add_l3_gw_T_add =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_l3_gw_result, add, "add");
static cmdline_parse_token_string_t cmd_tests_add_l3_gw_T_tests =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_l3_gw_result, tests, "tests");
static cmdline_parse_token_string_t cmd_tests_add_l3_gw_T_l3_intf =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_l3_gw_result, l3_gw, "l3_gw");
static cmdline_parse_token_string_t cmd_tests_add_l3_gw_T_port_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_l3_gw_result, port_kw, "port");
static cmdline_parse_token_num_t cmd_tests_add_l3_gw_T_port =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_add_l3_gw_result, port, UINT32);
static cmdline_parse_token_string_t cmd_tests_add_l3_gw_T_gw_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_l3_gw_result, gw_kw, "gw");
static cmdline_parse_token_ipaddr_t cmd_tests_add_l3_gw_T_gw =
    TOKEN_IPADDR_INITIALIZER(struct cmd_tests_add_l3_gw_result, gw);

static void cmd_tests_add_l3_gw_parsed(void *parsed_result, struct cmdline *cl,
                                       void *data __rte_unused)
{
    printer_arg_t                      parg;
    struct cmd_tests_add_l3_gw_result *pr;
    tpg_ip_t                           gw;

    parg = TPG_PRINTER_ARG(cli_printer, cl);
    pr = parsed_result;

    if (pr->gw.family != AF_INET) {
        cmdline_printf(cl, "ERROR: IPv6 not supported yet!\n");
        return;
    }

    gw.ip_v4 = rte_be_to_cpu_32(pr->gw.addr.ipv4.s_addr);
    if (test_mgmt_add_port_cfg_l3_gw(pr->port, &gw, &parg) == 0)
        cmdline_printf(cl, "Default gateway successfully added.\n");
    else
        cmdline_printf(cl, "ERROR: Failed to add default gateway!\n");
}

cmdline_parse_inst_t cmd_tests_add_l3_gw = {
    .f = cmd_tests_add_l3_gw_parsed,
    .data = NULL,
    .help_str = "add tests l3_gw port <port> gw <gw>",
    .tokens = {
        (void *)&cmd_tests_add_l3_gw_T_add,
        (void *)&cmd_tests_add_l3_gw_T_tests,
        (void *)&cmd_tests_add_l3_gw_T_l3_intf,
        (void *)&cmd_tests_add_l3_gw_T_port_kw,
        (void *)&cmd_tests_add_l3_gw_T_port,
        (void *)&cmd_tests_add_l3_gw_T_gw_kw,
        (void *)&cmd_tests_add_l3_gw_T_gw,
        NULL,
    },
};

/****************************************************************************
 * - "add tests server tcp|udp port <port>
      ips <ip_range> l4_ports <port_range>
      data-req-plen <len> data-resp-plen <len>"
 ****************************************************************************/
 struct cmd_tests_add_server_result {
    cmdline_fixed_string_t add;
    cmdline_fixed_string_t tests;
    cmdline_fixed_string_t tcp_udp;
    cmdline_fixed_string_t server;
    cmdline_fixed_string_t port_kw;
    uint32_t               port;
    cmdline_fixed_string_t tcid_kw;
    uint32_t               tcid;
    cmdline_fixed_string_t src;
    cmdline_ipaddr_t       src_low;
    cmdline_ipaddr_t       src_high;
    cmdline_fixed_string_t sports;
    uint16_t               sport_low;
    uint16_t               sport_high;

    cmdline_fixed_string_t data_req_plen_kw;
    uint16_t               data_req_plen;
    cmdline_fixed_string_t data_resp_plen_kw;
    uint16_t               data_resp_plen;
};

static cmdline_parse_token_string_t cmd_tests_add_server_T_add =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_server_result, add, "add");
static cmdline_parse_token_string_t cmd_tests_add_server_T_tests =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_server_result, tests, "tests");

static cmdline_parse_token_string_t cmd_tests_add_server_T_tcp_udp =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_server_result, tcp_udp, "tcp#udp");

static cmdline_parse_token_string_t cmd_tests_add_server_T_server =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_server_result, server, "server");
static cmdline_parse_token_string_t cmd_tests_add_server_T_port_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_server_result, port_kw, "port");
static cmdline_parse_token_num_t cmd_tests_add_server_T_port =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_add_server_result, port, UINT32);

static cmdline_parse_token_string_t cmd_tests_add_server_T_tcid_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_server_result, tcid_kw, "test-case-id");
static cmdline_parse_token_num_t cmd_tests_add_server_T_tcid =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_add_server_result, tcid, UINT32);

static cmdline_parse_token_string_t cmd_tests_add_server_T_src =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_server_result, src, "src");

static cmdline_parse_token_ipaddr_t cmd_tests_add_server_T_src_low =
    TOKEN_IPADDR_INITIALIZER(struct cmd_tests_add_server_result, src_low);
static cmdline_parse_token_ipaddr_t cmd_tests_add_server_T_src_high =
    TOKEN_IPADDR_INITIALIZER(struct cmd_tests_add_server_result, src_high);

static cmdline_parse_token_string_t cmd_tests_add_server_T_sports =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_server_result, sports, "sport");
static cmdline_parse_token_num_t cmd_tests_add_server_T_sport_low =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_add_server_result, sport_low, UINT16);
static cmdline_parse_token_num_t cmd_tests_add_server_T_sport_high =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_add_server_result, sport_high, UINT16);

static void cmd_tests_add_tcp_udp_server_parsed(void *parsed_result,
                                                struct cmdline *cl,
                                                void *data __rte_unused)
{
    printer_arg_t                       parg;
    struct cmd_tests_add_server_result *pr;
    tpg_test_case_t                     tc;

    parg = TPG_PRINTER_ARG(cli_printer, cl);
    pr = parsed_result;

    if (pr->src_low.family != AF_INET || pr->src_high.family != AF_INET) {
        cmdline_printf(cl, "ERROR: IPv6 not supported yet!\n");
        return;
    }

    test_init_defaults(&tc, TEST_CASE_TYPE__SERVER, pr->port, pr->tcid);

    if (strncmp(pr->tcp_udp, "tcp", strlen("tcp")) == 0)
        tc.tc_server.srv_l4.l4s_proto = L4_PROTO__TCP;
    else if (strncmp(pr->tcp_udp, "udp", strlen("udp")) == 0)
        tc.tc_server.srv_l4.l4s_proto = L4_PROTO__UDP;
    else
        assert(false);

    tc.tc_server.srv_ips =
        TPG_IPV4_RANGE(rte_be_to_cpu_32(pr->src_low.addr.ipv4.s_addr),
                       rte_be_to_cpu_32(pr->src_high.addr.ipv4.s_addr));
    tc.tc_server.srv_l4.l4s_tcp_udp.tus_ports =
        TPG_PORT_RANGE(pr->sport_low, pr->sport_high);

    if (test_mgmt_add_test_case(pr->port, &tc, &parg) == 0)
        cmdline_printf(cl,
                       "Test case %"PRIu32
                       " successfully added to port %"PRIu32"!\n",
                       pr->tcid, pr->port);
    else
        cmdline_printf(cl,
                       "ERROR: Failed to add test case %"PRIu32
                       " to port %"PRIu32"!\n",
                       pr->tcid, pr->port);
}

cmdline_parse_inst_t cmd_tests_add_tcp_udp_server = {
    .f = cmd_tests_add_tcp_udp_server_parsed,
    .data = NULL,
    .help_str = "add tests server tcp|udp port <port> "
                "src <ip_range> sport <port_range>",
    .tokens = {
        (void *)&cmd_tests_add_server_T_add,
        (void *)&cmd_tests_add_server_T_tests,
        (void *)&cmd_tests_add_server_T_server,
        (void *)&cmd_tests_add_server_T_tcp_udp,
        (void *)&cmd_tests_add_server_T_port_kw,
        (void *)&cmd_tests_add_server_T_port,
        (void *)&cmd_tests_add_server_T_tcid_kw,
        (void *)&cmd_tests_add_server_T_tcid,
        (void *)&cmd_tests_add_server_T_src,
        (void *)&cmd_tests_add_server_T_src_low,
        (void *)&cmd_tests_add_server_T_src_high,
        (void *)&cmd_tests_add_server_T_sports,
        (void *)&cmd_tests_add_server_T_sport_low,
        (void *)&cmd_tests_add_server_T_sport_high,
        NULL,
    },
};

/****************************************************************************
 * - "add tests client tcp|udp port <port> test-case-id <tcid>
        srcs <srcs_range> sports <sports>
        dests <rip> dports <rports>"
 ****************************************************************************/
 struct cmd_tests_add_client_result {
    cmdline_fixed_string_t add;
    cmdline_fixed_string_t tests;
    cmdline_fixed_string_t client;
    cmdline_fixed_string_t tcp_udp;
    cmdline_fixed_string_t port_kw;
    uint32_t               port;
    cmdline_fixed_string_t tcid_kw;
    uint32_t               tcid;

    cmdline_fixed_string_t srcs;
    cmdline_ipaddr_t       src_low;
    cmdline_ipaddr_t       src_high;
    cmdline_fixed_string_t sports;
    uint16_t               sport_low;
    uint16_t               sport_high;

    cmdline_fixed_string_t dests;
    cmdline_ipaddr_t       dest_low;
    cmdline_ipaddr_t       dest_high;
    cmdline_fixed_string_t dports;
    uint16_t               dport_low;
    uint16_t               dport_high;
};

static cmdline_parse_token_string_t cmd_tests_add_client_T_add =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_client_result, add, "add");
static cmdline_parse_token_string_t cmd_tests_add_client_T_tests =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_client_result, tests, "tests");

static cmdline_parse_token_string_t cmd_tests_add_client_T_client =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_client_result, client, "client");
static cmdline_parse_token_string_t cmd_tests_add_client_T_tcp_udp =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_client_result, tcp_udp, "tcp#udp");

static cmdline_parse_token_string_t cmd_tests_add_client_T_port_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_client_result, port_kw, "port");
static cmdline_parse_token_num_t cmd_tests_add_client_T_port =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_add_client_result, port, UINT32);

static cmdline_parse_token_string_t cmd_tests_add_client_T_tcid_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_client_result, tcid_kw, "test-case-id");
static cmdline_parse_token_num_t cmd_tests_add_client_T_tcid =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_add_client_result, tcid, UINT32);

static cmdline_parse_token_string_t cmd_tests_add_client_T_srcs =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_client_result, srcs, "src");
static cmdline_parse_token_ipaddr_t cmd_tests_add_client_T_src_low =
    TOKEN_IPADDR_INITIALIZER(struct cmd_tests_add_client_result, src_low);
static cmdline_parse_token_ipaddr_t cmd_tests_add_client_T_src_high =
    TOKEN_IPADDR_INITIALIZER(struct cmd_tests_add_client_result, src_high);

static cmdline_parse_token_string_t cmd_tests_add_client_T_sports =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_client_result, sports, "sport");
static cmdline_parse_token_num_t cmd_tests_add_client_T_sport_low =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_add_client_result, sport_low, UINT16);
static cmdline_parse_token_num_t cmd_tests_add_client_T_sport_high =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_add_client_result, sport_high, UINT16);

static cmdline_parse_token_string_t cmd_tests_add_client_T_dests =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_client_result, dests, "dest");
static cmdline_parse_token_ipaddr_t cmd_tests_add_client_T_dest_low =
    TOKEN_IPADDR_INITIALIZER(struct cmd_tests_add_client_result, dest_low);
static cmdline_parse_token_ipaddr_t cmd_tests_add_client_T_dest_high =
    TOKEN_IPADDR_INITIALIZER(struct cmd_tests_add_client_result, dest_high);

static cmdline_parse_token_string_t cmd_tests_add_client_T_dports =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_add_client_result, dports, "dport");
static cmdline_parse_token_num_t cmd_tests_add_client_T_dport_low =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_add_client_result, dport_low, UINT16);
static cmdline_parse_token_num_t cmd_tests_add_client_T_dport_high =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_add_client_result, dport_high, UINT16);

static void cmd_tests_add_client_parsed(void *parsed_result, struct cmdline *cl,
                                        void *data __rte_unused)
{
    printer_arg_t                       parg;
    struct cmd_tests_add_client_result *pr;
    tpg_test_case_t                     tc;

    parg = TPG_PRINTER_ARG(cli_printer, cl);
    pr = parsed_result;

    if (pr->src_low.family != AF_INET ||
            pr->src_high.family != AF_INET ||
            pr->dest_low.family != AF_INET ||
            pr->dest_high.family != AF_INET) {
        cmdline_printf(cl, "ERROR: IPv6 not supported yet!\n");
        return;
    }

    test_init_defaults(&tc, TEST_CASE_TYPE__CLIENT, pr->port, pr->tcid);

    if (strncmp(pr->tcp_udp, "tcp", strlen("tcp")) == 0)
        tc.tc_client.cl_l4.l4c_proto = L4_PROTO__TCP;
    else if (strncmp(pr->tcp_udp, "udp", strlen("udp")) == 0)
        tc.tc_client.cl_l4.l4c_proto = L4_PROTO__UDP;

    tc.tc_client.cl_src_ips =
        TPG_IPV4_RANGE(rte_be_to_cpu_32(pr->src_low.addr.ipv4.s_addr),
                       rte_be_to_cpu_32(pr->src_high.addr.ipv4.s_addr));
    tc.tc_client.cl_l4.l4c_tcp_udp.tuc_sports =
        TPG_PORT_RANGE(pr->sport_low, pr->sport_high);

    tc.tc_client.cl_dst_ips =
        TPG_IPV4_RANGE(rte_be_to_cpu_32(pr->dest_low.addr.ipv4.s_addr),
                       rte_be_to_cpu_32(pr->dest_high.addr.ipv4.s_addr));
    tc.tc_client.cl_l4.l4c_tcp_udp.tuc_dports =
        TPG_PORT_RANGE(pr->dport_low, pr->dport_high);

    if (test_mgmt_add_test_case(pr->port, &tc, &parg) == 0)
        cmdline_printf(cl,
                       "Test case %"PRIu32
                       " successfully added to port %"PRIu32"!\n",
                       pr->tcid, pr->port);
    else
        cmdline_printf(cl,
                       "ERROR: Failed to add test case %"PRIu32
                       " to port %"PRIu32"!\n",
                       pr->tcid, pr->port);
}

cmdline_parse_inst_t cmd_tests_add_client = {
    .f = cmd_tests_add_client_parsed,
    .data = NULL,
    .help_str = "add tests client tcp|udp port <port> test-case-id <tcid> "
                "src <ip-range> sport <l4-ports> "
                "dest <ip-range> dport <l4-ports>",
    .tokens = {
        (void *)&cmd_tests_add_client_T_add,
        (void *)&cmd_tests_add_client_T_tests,
        (void *)&cmd_tests_add_client_T_client,
        (void *)&cmd_tests_add_client_T_tcp_udp,
        (void *)&cmd_tests_add_client_T_port_kw,
        (void *)&cmd_tests_add_client_T_port,
        (void *)&cmd_tests_add_client_T_tcid_kw,
        (void *)&cmd_tests_add_client_T_tcid,

        (void *)&cmd_tests_add_client_T_srcs,
        (void *)&cmd_tests_add_client_T_src_low,
        (void *)&cmd_tests_add_client_T_src_high,

        (void *)&cmd_tests_add_client_T_sports,
        (void *)&cmd_tests_add_client_T_sport_low,
        (void *)&cmd_tests_add_client_T_sport_high,

        (void *)&cmd_tests_add_client_T_dests,
        (void *)&cmd_tests_add_client_T_dest_low,
        (void *)&cmd_tests_add_client_T_dest_high,

        (void *)&cmd_tests_add_client_T_dports,
        (void *)&cmd_tests_add_client_T_dport_low,
        (void *)&cmd_tests_add_client_T_dport_high,
        NULL,
    },
};

/****************************************************************************
 * - "del tests port <port> test-case-id <tcid>
 ****************************************************************************/
struct cmd_tests_del_test_result {
    cmdline_fixed_string_t del;
    cmdline_fixed_string_t tests;
    cmdline_fixed_string_t port_kw;
    uint32_t               port;
    cmdline_fixed_string_t tcid_kw;
    uint32_t               tcid;
};

static cmdline_parse_token_string_t cmd_tests_del_test_T_del =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_del_test_result, del, "del");
static cmdline_parse_token_string_t cmd_tests_del_test_T_tests =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_del_test_result, tests, "tests");

static cmdline_parse_token_string_t cmd_tests_del_test_T_port_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_del_test_result, port_kw, "port");
static cmdline_parse_token_num_t cmd_tests_del_test_T_port =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_del_test_result, port, UINT32);

static cmdline_parse_token_string_t cmd_tests_del_test_T_tcid_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_del_test_result, tcid_kw, "test-case-id");
static cmdline_parse_token_num_t cmd_tests_del_test_T_tcid =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_del_test_result, tcid, UINT32);

static void cmd_tests_del_test_parsed(void *parsed_result, struct cmdline *cl,
                                      void *data __rte_unused)
{
    printer_arg_t                     parg;
    struct cmd_tests_del_test_result *pr;

    parg = TPG_PRINTER_ARG(cli_printer, cl);
    pr = parsed_result;

    if (test_mgmt_del_test_case(pr->port, pr->tcid, &parg) == 0)
        cmdline_printf(cl,
                       "Successfully deleted test case %"PRIu32
                       " from port %"PRIu32"\n",
                       pr->tcid,
                       pr->port);
    else
        cmdline_printf(cl,
                       "ERROR: Failed to delete test case %"PRIu32
                       " from port %"PRIu32"\n",
                       pr->tcid,
                       pr->port);
}

cmdline_parse_inst_t cmd_tests_del_test = {
    .f = cmd_tests_del_test_parsed,
    .data = NULL,
    .help_str = "del tests port <port> test-case-id <tcid>",
    .tokens = {
        (void *)&cmd_tests_del_test_T_del,
        (void *)&cmd_tests_del_test_T_tests,
        (void *)&cmd_tests_del_test_T_port_kw,
        (void *)&cmd_tests_del_test_T_port,
        (void *)&cmd_tests_del_test_T_tcid_kw,
        (void *)&cmd_tests_del_test_T_tcid,
        NULL,
    },
};

/****************************************************************************
 * - "set tests rate port <port> test-case-id <tcid>
 *      open|close|send <rate> | infinite"
 ****************************************************************************/
 struct cmd_tests_set_rate_result {
    cmdline_fixed_string_t set;
    cmdline_fixed_string_t tests;
    cmdline_fixed_string_t rate;
    cmdline_fixed_string_t port_kw;
    uint32_t               port;
    cmdline_fixed_string_t tcid_kw;
    uint32_t               tcid;

    cmdline_fixed_string_t rate_kw;
    uint32_t               rate_val;

    cmdline_fixed_string_t infinite;
};

static cmdline_parse_token_string_t cmd_tests_set_rate_T_set =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_rate_result, set, "set");
static cmdline_parse_token_string_t cmd_tests_set_rate_T_tests =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_rate_result, tests, "tests");
static cmdline_parse_token_string_t cmd_tests_set_rate_T_rate =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_rate_result, rate, "rate");

static cmdline_parse_token_string_t cmd_tests_set_rate_T_port_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_rate_result, port_kw, "port");
static cmdline_parse_token_num_t cmd_tests_set_rate_T_port =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_set_rate_result, port, UINT32);

static cmdline_parse_token_string_t cmd_tests_set_rate_T_tcid_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_rate_result, tcid_kw, "test-case-id");
static cmdline_parse_token_num_t cmd_tests_set_rate_T_tcid =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_set_rate_result, tcid, UINT32);

static cmdline_parse_token_string_t cmd_tests_set_rate_T_rate_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_rate_result, rate_kw, "open#close#send");
static cmdline_parse_token_num_t cmd_tests_set_rate_T_rate_val =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_set_rate_result, rate_val, UINT32);

static cmdline_parse_token_string_t cmd_tests_set_rate_T_infinite =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_rate_result, infinite, "infinite");

static void cmd_tests_set_rate_parsed(void *parsed_result, struct cmdline *cl,
                                      void *data)
{
    printer_arg_t                     parg;
    struct cmd_tests_set_rate_result *pr;
    bool                              infinite = (((intptr_t) data) == 'i');
    tpg_rate_t                        rate;
    tpg_rate_type_t                   rate_type;

    parg = TPG_PRINTER_ARG(cli_printer, cl);
    pr = parsed_result;


    if (strncmp(pr->rate_kw, "open", strlen("open")) == 0)
        rate_type = RATE_TYPE__OPEN;
    else if (strncmp(pr->rate_kw, "close", strlen("close")) == 0)
        rate_type = RATE_TYPE__CLOSE;
    else if (strncmp(pr->rate_kw, "send", strlen("send")) == 0)
        rate_type = RATE_TYPE__SEND;
    else
        assert(false);

    if (infinite)
        rate = TPG_RATE_INF();
    else
        rate = TPG_RATE(pr->rate_val);

    if (test_mgmt_update_test_case_rate(pr->port, pr->tcid, rate_type,
                                        &rate, &parg) == 0)
        cmdline_printf(cl, "Port %"PRIu32", Test Case %"PRIu32" updated!\n",
                       pr->port,
                       pr->tcid);
    else
        cmdline_printf(cl,
                       "ERROR: Failed updating test case %"PRIu32
                       " config on port %"PRIu32"\n",
                       pr->tcid,
                       pr->port);
}

cmdline_parse_inst_t cmd_tests_set_rate = {
    .f = cmd_tests_set_rate_parsed,
    .data = NULL,
    .help_str = "set tests rate port <port> test-case-id <tcid> "
                "open|close|send <rate>",
    .tokens = {
        (void *)&cmd_tests_set_rate_T_set,
        (void *)&cmd_tests_set_rate_T_tests,
        (void *)&cmd_tests_set_rate_T_rate,
        (void *)&cmd_tests_set_rate_T_port_kw,
        (void *)&cmd_tests_set_rate_T_port,
        (void *)&cmd_tests_set_rate_T_tcid_kw,
        (void *)&cmd_tests_set_rate_T_tcid,
        (void *)&cmd_tests_set_rate_T_rate_kw,
        (void *)&cmd_tests_set_rate_T_rate_val,
        NULL,
    },
};

cmdline_parse_inst_t cmd_tests_set_rate_infinite = {
    .f = cmd_tests_set_rate_parsed,
    .data = (void *) (intptr_t) 'i',
    .help_str = "set tests rate port <port> test-case-id <tcid> "
                "open|close|send infinite",
    .tokens = {
        (void *)&cmd_tests_set_rate_T_set,
        (void *)&cmd_tests_set_rate_T_tests,
        (void *)&cmd_tests_set_rate_T_rate,
        (void *)&cmd_tests_set_rate_T_port_kw,
        (void *)&cmd_tests_set_rate_T_port,
        (void *)&cmd_tests_set_rate_T_tcid_kw,
        (void *)&cmd_tests_set_rate_T_tcid,
        (void *)&cmd_tests_set_rate_T_rate_kw,
        (void *)&cmd_tests_set_rate_T_infinite,
        NULL,
    },
};

/****************************************************************************
 * - "set tests timeouts port <port> test-case-id <tcid>
 *      init|uptime|downtime <timeout>|infinite
 ****************************************************************************/
 struct cmd_tests_set_timeouts_result {
    cmdline_fixed_string_t set;
    cmdline_fixed_string_t tests;
    cmdline_fixed_string_t timeouts;
    cmdline_fixed_string_t port_kw;
    uint32_t               port;
    cmdline_fixed_string_t tcid_kw;
    uint32_t               tcid;

    cmdline_fixed_string_t timeout_kw;
    uint32_t               timeout;

    cmdline_fixed_string_t infinite;
};

static cmdline_parse_token_string_t cmd_tests_set_timeouts_T_set =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_timeouts_result, set, "set");
static cmdline_parse_token_string_t cmd_tests_set_timeouts_T_tests =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_timeouts_result, tests, "tests");
static cmdline_parse_token_string_t cmd_tests_set_timeouts_T_timeouts =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_timeouts_result, timeouts, "timeouts");

static cmdline_parse_token_string_t cmd_tests_set_timeouts_T_port_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_timeouts_result, port_kw, "port");
static cmdline_parse_token_num_t cmd_tests_set_timeouts_T_port =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_set_timeouts_result, port, UINT32);

static cmdline_parse_token_string_t cmd_tests_set_timeouts_T_tcid_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_timeouts_result, tcid_kw, "test-case-id");
static cmdline_parse_token_num_t cmd_tests_set_timeouts_T_tcid =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_set_timeouts_result, tcid, UINT32);

static cmdline_parse_token_string_t cmd_tests_set_timeouts_T_timeout_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_timeouts_result, timeout_kw, "init#uptime#downtime");
static cmdline_parse_token_num_t cmd_tests_set_timeouts_T_timeout =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_set_timeouts_result, timeout, UINT32);

static cmdline_parse_token_string_t cmd_tests_set_timeouts_T_infinite =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_timeouts_result, infinite, "infinite");

static void cmd_tests_set_timeouts_parsed(void *parsed_result,
                                          struct cmdline *cl,
                                          void *data)
{
    printer_arg_t                         parg;
    struct cmd_tests_set_timeouts_result *pr;
    bool                                  infinite;
    tpg_delay_t                           timeout;
    tpg_delay_type_t                      timeout_type;

    parg = TPG_PRINTER_ARG(cli_printer, cl);
    pr = parsed_result;
    infinite = (((intptr_t) data) == 'i');

    if (strncmp(pr->timeout_kw, "init", strlen("init")) == 0)
        timeout_type = DELAY_TYPE__INIT;
    else if (strncmp(pr->timeout_kw, "uptime", strlen("uptime")) == 0)
        timeout_type = DELAY_TYPE__UPTIME;
    else if (strncmp(pr->timeout_kw, "downtime", strlen("downtime")) == 0)
        timeout_type = DELAY_TYPE__DOWNTIME;
    else
        assert(false);

    if (infinite)
        timeout = TPG_DELAY_INF();
    else
        timeout = TPG_DELAY(pr->timeout);

    if (test_mgmt_update_test_case_timeout(pr->port, pr->tcid, timeout_type,
                                           &timeout,
                                           &parg) == 0)
        cmdline_printf(cl, "Port %"PRIu32", Test Case %"PRIu32" updated!\n",
                       pr->port,
                       pr->tcid);
    else
        cmdline_printf(cl,
                       "ERROR: Failed updating test case %"PRIu32
                       " config on port %"PRIu32"\n",
                       pr->tcid,
                       pr->port);
}

cmdline_parse_inst_t cmd_tests_set_timeouts = {
    .f = cmd_tests_set_timeouts_parsed,
    .data = NULL,
    .help_str = "set tests timeouts port <port> test-case-id <tcid> "
                "init|uptime|downtime <rate>",
    .tokens = {
        (void *)&cmd_tests_set_timeouts_T_set,
        (void *)&cmd_tests_set_timeouts_T_tests,
        (void *)&cmd_tests_set_timeouts_T_timeouts,
        (void *)&cmd_tests_set_timeouts_T_port_kw,
        (void *)&cmd_tests_set_timeouts_T_port,
        (void *)&cmd_tests_set_timeouts_T_tcid_kw,
        (void *)&cmd_tests_set_timeouts_T_tcid,
        (void *)&cmd_tests_set_timeouts_T_timeout_kw,
        (void *)&cmd_tests_set_timeouts_T_timeout,
        NULL,
    },
};

cmdline_parse_inst_t cmd_tests_set_timeouts_infinite = {
    .f = cmd_tests_set_timeouts_parsed,
    .data = (void *) (intptr_t) 'i',
    .help_str = "set tests timeouts port <port> test-case-id <tcid> "
                "init|uptime|downtime infinite",
    .tokens = {
        (void *)&cmd_tests_set_timeouts_T_set,
        (void *)&cmd_tests_set_timeouts_T_tests,
        (void *)&cmd_tests_set_timeouts_T_timeouts,
        (void *)&cmd_tests_set_timeouts_T_port_kw,
        (void *)&cmd_tests_set_timeouts_T_port,
        (void *)&cmd_tests_set_timeouts_T_tcid_kw,
        (void *)&cmd_tests_set_timeouts_T_tcid,
        (void *)&cmd_tests_set_timeouts_T_timeout_kw,
        (void *)&cmd_tests_set_timeouts_T_infinite,
        NULL,
    },
};

/****************************************************************************
 * - "set tests criteria port <port> test-case-id <tcid>
 *      run-time|servers-up|clients-up|clients-estab|data-MB <value>"
 ****************************************************************************/
 struct cmd_tests_set_criteria_result {
    cmdline_fixed_string_t set;
    cmdline_fixed_string_t tests;
    cmdline_fixed_string_t criteria;
    cmdline_fixed_string_t port_kw;
    uint32_t               port;
    cmdline_fixed_string_t tcid_kw;
    uint32_t               tcid;

    cmdline_fixed_string_t criteria_kw;
    uint32_t               criteria_val;
};

static cmdline_parse_token_string_t cmd_tests_set_criteria_T_set =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_criteria_result, set, "set");
static cmdline_parse_token_string_t cmd_tests_set_criteria_T_tests =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_criteria_result, tests, "tests");
static cmdline_parse_token_string_t cmd_tests_set_criteria_T_criteria =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_criteria_result, criteria, "criteria");

static cmdline_parse_token_string_t cmd_tests_set_criteria_T_port_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_criteria_result, port_kw, "port");
static cmdline_parse_token_num_t cmd_tests_set_criteria_T_port =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_set_criteria_result, port, UINT32);

static cmdline_parse_token_string_t cmd_tests_set_criteria_T_tcid_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_criteria_result, tcid_kw, "test-case-id");
static cmdline_parse_token_num_t cmd_tests_set_criteria_T_tcid =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_set_criteria_result, tcid, UINT32);

static cmdline_parse_token_string_t cmd_tests_set_criteria_T_criteria_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_criteria_result, criteria_kw, "run-time#servers-up#clients-up#clients-estab#data-MB");
static cmdline_parse_token_num_t cmd_tests_set_criteria_T_criteria_val =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_set_criteria_result, criteria_val, UINT32);

static void cmd_tests_set_criteria_parsed(void *parsed_result,
                                          struct cmdline *cl,
                                          void *data __rte_unused)
{
    printer_arg_t                         parg;
    struct cmd_tests_set_criteria_result *pr;
    tpg_test_criteria_t                   criteria;

    parg = TPG_PRINTER_ARG(cli_printer, cl);
    pr = parsed_result;

    if (strncmp(pr->criteria_kw, "run-time", strlen("run-time")) == 0)
        criteria = CRIT_RUN_TIME(pr->criteria_val);
    else if (strncmp(pr->criteria_kw, "servers-up", strlen("servers-up")) == 0)
        criteria = CRIT_SRV_UP(pr->criteria_val);
    else if (strncmp(pr->criteria_kw, "clients-up", strlen("clients-up")) == 0)
        criteria = CRIT_CL_UP(pr->criteria_val);
    else if (strncmp(pr->criteria_kw, "clients-estab", strlen("clients-estab")) == 0)
        criteria = CRIT_CL_ESTAB(pr->criteria_val);
    else if (strncmp(pr->criteria_kw, "data-MB", strlen("data-MB")) == 0)
        criteria = CRIT_DATA_MB(pr->criteria_val);
    else
        assert(false);

    if (test_mgmt_update_test_case_criteria(pr->port, pr->tcid, &criteria,
                                            &parg) == 0)
        cmdline_printf(cl, "Port %"PRIu32", Test Case %"PRIu32" updated!\n",
                       pr->port,
                       pr->tcid);
    else
        cmdline_printf(cl,
                       "ERROR: Failed updating test case %"PRIu32
                       " config on port %"PRIu32"\n",
                       pr->tcid,
                       pr->port);
}

cmdline_parse_inst_t cmd_tests_set_criteria = {
    .f = cmd_tests_set_criteria_parsed,
    .data = NULL,
    .help_str = "set tests criteria port <port> test-case-id <tcid> "
                 "run-time|servers-up|clients-up|clients-estab|data-MB <value>",
    .tokens = {
        (void *)&cmd_tests_set_criteria_T_set,
        (void *)&cmd_tests_set_criteria_T_tests,
        (void *)&cmd_tests_set_criteria_T_criteria,
        (void *)&cmd_tests_set_criteria_T_port_kw,
        (void *)&cmd_tests_set_criteria_T_port,
        (void *)&cmd_tests_set_criteria_T_tcid_kw,
        (void *)&cmd_tests_set_criteria_T_tcid,
        (void *)&cmd_tests_set_criteria_T_criteria_kw,
        (void *)&cmd_tests_set_criteria_T_criteria_val,
        NULL,
    },
};

/****************************************************************************
 * - "set tests async port <port> test-case-id <tcid>""
 ****************************************************************************/
 struct cmd_tests_set_async_result {
    cmdline_fixed_string_t set;
    cmdline_fixed_string_t tests;
    cmdline_fixed_string_t async;
    cmdline_fixed_string_t port_kw;
    uint32_t               port;
    cmdline_fixed_string_t tcid_kw;
    uint32_t               tcid;
};

static cmdline_parse_token_string_t cmd_tests_set_async_T_set =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_async_result, set, "set");
static cmdline_parse_token_string_t cmd_tests_set_async_T_tests =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_async_result, tests, "tests");
static cmdline_parse_token_string_t cmd_tests_set_async_T_async =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_async_result, async, "async");

static cmdline_parse_token_string_t cmd_tests_set_async_T_port_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_async_result, port_kw, "port");
static cmdline_parse_token_num_t cmd_tests_set_async_T_port =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_set_async_result, port, UINT32);

static cmdline_parse_token_string_t cmd_tests_set_async_T_tcid_kw =
    TOKEN_STRING_INITIALIZER(struct cmd_tests_set_async_result, tcid_kw, "test-case-id");
static cmdline_parse_token_num_t cmd_tests_set_async_T_tcid =
    TOKEN_NUM_INITIALIZER(struct cmd_tests_set_async_result, tcid, UINT32);

static void cmd_tests_set_async_parsed(void *parsed_result,
                                       struct cmdline *cl,
                                       void *data __rte_unused)
{
    printer_arg_t                      parg;
    struct cmd_tests_set_async_result *pr;

    parg = TPG_PRINTER_ARG(cli_printer, cl);
    pr = parsed_result;

    if (test_mgmt_update_test_case_async(pr->port, pr->tcid, true, &parg) == 0)
        cmdline_printf(cl, "Port %"PRIu32", Test Case %"PRIu32" updated!\n",
                       pr->port,
                       pr->tcid);
    else
        cmdline_printf(cl,
                       "ERROR: Failed updating test case %"PRIu32
                       " config on port %"PRIu32"\n",
                       pr->tcid,
                       pr->port);
}

cmdline_parse_inst_t cmd_tests_set_noasync = {
    .f = cmd_tests_set_async_parsed,
    .data = NULL,
    .help_str = "set tests async port <port> test-case-id <tcid>",
    .tokens = {
        (void *)&cmd_tests_set_async_T_set,
        (void *)&cmd_tests_set_async_T_tests,
        (void *)&cmd_tests_set_async_T_async,
        (void *)&cmd_tests_set_async_T_port_kw,
        (void *)&cmd_tests_set_async_T_port,
        (void *)&cmd_tests_set_async_T_tcid_kw,
        (void *)&cmd_tests_set_async_T_tcid,
        NULL,
    },
};

/*****************************************************************************
 * Main menu context
 ****************************************************************************/
static cmdline_parse_ctx_t cli_ctx[] = {
    &cmd_tests_start,
    &cmd_tests_stop,
    &cmd_show_tests_ui,
    &cmd_show_tests_config,
    &cmd_show_tests_state,
    &cmd_show_tests_stats,
    &cmd_tests_add_l3_intf,
    &cmd_tests_add_l3_gw,
    &cmd_tests_add_tcp_udp_server,
    &cmd_tests_add_client,
    &cmd_tests_del_test,
    &cmd_tests_set_rate,
    &cmd_tests_set_rate_infinite,
    &cmd_tests_set_timeouts,
    &cmd_tests_set_timeouts_infinite,
    &cmd_tests_set_criteria,
    &cmd_tests_set_noasync,
    NULL,
};

/*****************************************************************************
 * test_mgmt_cli_init()
 ****************************************************************************/
bool test_mgmt_cli_init(void)
{
    return cli_add_main_ctx(cli_ctx);
}

