import QuantLib as ql

# Parameters
underlying_price = 100.0
strike_price = 100.0
time_to_expiration = 1.0
risk_free_rate = 0.05
volatility = 0.20
v0 = 0.04
kappa = 2.0
theta = 0.04
sigma = 0.3
rho = -0.7

# option type
option_type = ql.Option.Put

# Heston process
spot_handle = ql.QuoteHandle(ql.SimpleQuote(underlying_price))
rate_handle = ql.YieldTermStructureHandle(ql.FlatForward(0, ql.NullCalendar(), ql.QuoteHandle(ql.SimpleQuote(risk_free_rate)), ql.Actual365Fixed()))

heston_process = ql.HestonProcess(rate_handle, rate_handle, spot_handle, v0, kappa, theta, sigma, rho)
heston_model = ql.HestonModel(heston_process)
engine = ql.AnalyticHestonEngine(heston_model)

# create option
payoff = ql.PlainVanillaPayoff(option_type, strike_price)
exercise = ql.EuropeanExercise(ql.Date().todaysDate() + int(time_to_expiration * 365))
option = ql.VanillaOption(payoff, exercise)
option.setPricingEngine(engine)

# calculate option price
option_price = option.NPV()
print(f"Option Price: {option_price}")
