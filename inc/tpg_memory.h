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
 *     tpg_memory.h
 *
 * Description:
 *     Common place where memory and buffer pools get allocated.
 *
 * Author:
 *     Dumitru Ceara, Eelco Chaudron
 *
 * Initial Created:
 *     06/29/2015
 *
 * Notes:
 *
 */

/*****************************************************************************
 * Multiple include protection
 ****************************************************************************/
#ifndef _H_TPG_MEMORY_
#define _H_TPG_MEMORY_

/*****************************************************************************
 * External's for tpg_memory.c
 ****************************************************************************/
RTE_DECLARE_PER_LCORE(struct rte_mempool *, mbuf_pool);
RTE_DECLARE_PER_LCORE(struct rte_mempool *, mbuf_pool_tx_hdr);
RTE_DECLARE_PER_LCORE(struct rte_mempool *, mbuf_pool_clone);
RTE_DECLARE_PER_LCORE(struct rte_mempool *, tcb_pool);
RTE_DECLARE_PER_LCORE(struct rte_mempool *, ucb_pool);

extern bool                 mem_init(void);
extern void                 mem_lcore_init(uint32_t lcore_id);
extern struct rte_mempool  *mem_get_mbuf_pool(uint32_t port, uint32_t queue_id);
extern struct rte_mempool **mem_get_tcb_pools(void);
extern struct rte_mempool **mem_get_ucb_pools(void);

/*****************************************************************************
 * mem_get_mbuf_local_pool()
 ****************************************************************************/
static inline __attribute__((always_inline))
struct rte_mempool *mem_get_mbuf_local_pool(void)
{
    return RTE_PER_LCORE(mbuf_pool);
}

/*****************************************************************************
 * mem_get_mbuf_local_pool_tx_hdr()
 ****************************************************************************/
static inline __attribute__((always_inline))
struct rte_mempool *mem_get_mbuf_local_pool_tx_hdr(void)
{
    return RTE_PER_LCORE(mbuf_pool_tx_hdr);
}

/*****************************************************************************
 * mem_get_mbuf_local_pool_clone()
 ****************************************************************************/
static inline __attribute__((always_inline))
struct rte_mempool *mem_get_mbuf_local_pool_clone(void)
{
    return RTE_PER_LCORE(mbuf_pool_clone);
}

/*****************************************************************************
 * mem_get_tcb_local_pool()
 ****************************************************************************/
static inline __attribute__((always_inline))
struct rte_mempool *mem_get_tcb_local_pool(void)
{
    return RTE_PER_LCORE(tcb_pool);
}

/*****************************************************************************
 * mem_get_ucb_local_pool()
 ****************************************************************************/
static inline __attribute__((always_inline))
struct rte_mempool *mem_get_ucb_local_pool(void)
{
    return RTE_PER_LCORE(ucb_pool);
}

#endif /* _H_TPG_MEMORY_ */

