volatile uint16_t min_value = 4095;
volatile uint16_t max_value = 0;
volatile uint16_t t = 0;

inline uint16_t clip(uint16_t value, uint16_t min_value, uint16_t max_value) {
    if (value >= min_value && value <= max_value) {
      return value;
    } else if (value < min_value) {
      return min_value;
    } else {
      return max_value;
    }
}

inline uint16_t fuzz(uint16_t value) {
    if (value > max_value) {
      max_value = value;
    } else if (value < min_value) {
      min_value = value;
    }

    uint16_t half_point = min_value + ((max_value - min_value) / 2);
    
    value = clip(value, half_point - 200, half_point + 200); 
    
    if (t == 6500) {
        min_value = half_point;
        max_value = half_point;
        t = 0;
    }

    t += 1;

    return clip(value * 2, 0, 4096);
}