/**
 * @brief           amplitude limter filter
 *
 * @param[in]       new_value: new value.
 * @param[in]       old_value: old value.
 *
 * @return
 *                  - filter result
 *
 */
#define LIMIT   10
uint8_t amplitude_limiter_filter(uint8_t new_value, uint8_t old_value)
{
    if ((new_value - old_value) > LIMIT || ((old_value - new_value) < LIMIT))
    {
        return old_value;
    }
    else
    {
        return new_value;
    }     
}


/**
 * @brief           middle value filter
 *
 * @param[in]       buf: pointer to the buffer where the original sampling are 
 *                  stored.
 * @param[in]       len: size(in bytes) in buf.
 *
 * @return
 *                  - filter result
 *
 */
uint8_t middle_value_filter(uint8_t *buf, uint8_t len)
{
    uint8_t tmp = 0;

    /* 冒泡排序 */
    for (uint8_t j = 0; j < len-1; j++)
    {
        for (uint8_t i = 0; i < len-1-j; i++)
        {
            if (buf[i] > buf[i+1])
            {
                tmp = buf[i];
                buf[i] = buf[i+1];
                buf[i+1] = tmp;
            }
        } 
    }

    return buf[(len-1)>>1];
}

/**
 * @brief           arithmetical average value filter
 *
 * @param[in]       buf: pointer to the buffer where the original sampling are 
 *                  stored.
 * @param[in]       len: size(in bytes) in buf.
 *
 * @return
 *                  - filter result
 *
 */
uint8_t arithmetical_average_value_filter(uint8_t *buf, uint8_t len)
{
    uint16_t sum = 0;

    for (uint8_t i = 0; i < len; i++)
    {
        sum += buf[i];
    }
    
    return (sum + len / 2) / len;
}

/**
 * @brief           glide average value filter
 *
 * @param[in]       buf: pointer to the buffer where the original sampling are 
 *                  stored.
 * @param[in]       len: size(in bytes) in buf.
 * @param[in]       new_value: new sampling value.
 *
 * @return
 *                  - filter result
 *
 */
uint8_t glide_average_value_filter(uint8_t *buf, uint8_t len, uint8_t new_value)
{
    uint16_t sum = 0;

    for (uint8_t i = 0; i < len-1; i++)
    {
        buf[i] = buf[i+1];
        sum += buf[i];
    }
    buf[len-1] = new_value;
    sum += buf[len-1];

    return (sum + len / 2) / len;
}

/**
 * @brief           middle average value filter
 *
 * @param[in]       buf: pointer to the buffer where the original sampling are 
 *                  stored.
 * @param[in]       len: size(in bytes) in buf.
 *
 * @return
 *                  - filter result
 *
 */
uint8_t middle_average_value_filter(uint8_t *buf, uint8_t len)
{
    uint8_t tmp = 0;
    uint16_t sum = 0;

    /* 冒泡排序 */
    for (uint8_t j = 0; j < len-1; j++)
    {
        for (uint8_t i = 0; i < len-1-j; i++)
        {
            if (buf[i] > buf[i+1])
            {
                tmp = buf[i];
                buf[i] = buf[i+1];
                buf[i+1] = tmp;
            }
        } 
    }

    for (uint8_t i = 1; i < len-1; i++)
    {
        sum += buf[i];
    }

    return (sum + (len-2)/2) / (len-2);
}

/**
 * @brief           glide middle average value filter
 *
 * @param[in]       buf: pointer to the buffer where the original sampling are 
 *                  stored.
 * @param[in]       len: size(in bytes) in buf.
 * @param[in]       new_value: new sampling value.
 *
 * @return
 *                  - filter result
 *
 */
uint8_t glide_middle_average_value_filter(uint8_t *buf, uint8_t len, uint8_t new_value)
{
    uint8_t tmp = 0;
    uint8_t min = 0;
    uint8_t max = 0;
    uint16_t sum = 0;

    sum = max = min = new_value;

    for (uint8_t i = 0; i < len-1; i++)
    {
        buf[i] = buf[i+1];
        sum += buf[i];
        if (buf[i] > max) max = buf[i];
        else if(buf[i] < min) min = buf[i];
    }

    buf[len-1] = new_value;
    sum = sum - max - min;

    return (sum + (len-2)/2) / (len-2);
}

/**
 * @brief           limit glide average value filter
 *
 * @param[in]       buf: pointer to the buffer where the original sampling are 
 *                  stored.
 * @param[in]       len: size(in bytes) in buf.
 * @param[in]       new_value: new sampling value.
 *
 * @return
 *                  - filter result
 *
 */
uint8_t limit_glide_average_value_filter(uint8_t *buf, uint8_t len, uint8_t new_value)
{
    uint16_t sum = 0;

    if (new_value - buf[len-1] > LIMIT) || (buf[len-1] - new_value > LIMIT)
    {
        new_value = buf[len-1];
    }
    
    for (uint8_t i = 0; i < len-1; i++)
    {
        buf[i] = buf[i+1];
        sum += buf[i];
    }

    buf[len-1] = new_value;
    sum += buf[len-1];

    return (sum + len/2) / len;
    
    //return glide_average_value_filter(buf, len, new_value);
}

/**
 * @brief           weighted glide middle average value filter
 *
 * @param[in]       buf: pointer to the buffer where the original sampling are 
 *                  stored.
 * @param[in]       len: size(in bytes) in buf.
 * @param[in]       new_value: new sampling value. 
 * @param[in]       coe: weight coefficient, such as 
 *                  coe[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}.
 *
 * @return
 *                  - filter result
 *
 */
uint8_t weighted_glide_average_value_filter(uint8_t *buf, uint8_t len, uint8_t new_value, uint8_t *coe)
{
    uint16_t sum = 0;
    uint16_t coe_sum = 0;

    for (uint8_t i = 0; i < len-1; i++)
    {
        buf[i] = buf[i+1];
        sum += buf[i] * coe[i];
        coe_sum += coe[i];
    }

    buf[len-1] = new_value;
    sum += buf[len-1] * coe[len-1];
    coe_sum += coe[len-1];

    sum = (sum + coe_sum/2) / coe_sum;;

    return (sum + len/2) / len;
}

/**
 * @brief           eliminate dithering filter
 *
 * @param[in]       new_value: new sampling value. 
 * @param[in]       new_value: old sampling value.  
 *
 * @return
 *                  - filter result
 *
 */
#define ELIMINATE_DITHERING_TIMES   10
uint8_t eliminate_dithering_filter(uint8_t new_value, uint8_t old_value)
{
    

    for (uint8_t i = 0; i < ELIMINATE_DITHERING_TIMES; i++)
    {
        /* code */
    }
    
}