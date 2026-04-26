# Trading Journal

A personal WinUI 3 desktop app for logging and analyzing options and stock trades.

## Tech Stack

- **UI Framework**: WinUI 3 (Windows App SDK)
- **Runtime**: .NET 9+
- **Language**: C#
- **MVVM**: CommunityToolkit.Mvvm — all logic lives in ViewModels; no logic in code-behind
- **Charts**: LiveCharts2 (WinUI)
- **Database**: Microsoft.Data.Sqlite (local storage)

## Architecture

MVVM strict mode — code-behind files contain only UI wiring (InitializeComponent, DataContext assignment). All state, commands, and business logic belong in ViewModels.

```
TradingJournal/
├── Models/           # Plain data classes (Trade, OptionLeg, Strategy, etc.)
├── ViewModels/       # CommunityToolkit.Mvvm ObservableObject subclasses
├── Views/            # XAML pages and controls (minimal code-behind)
├── Services/         # Data access, SQLite repository, business logic
├── Contracts/        # Interfaces for services (ITradeRepository, etc.)
└── Assets/           # Icons, images
```

## Domain

### Trade Types

- **Stock trades**: long/short, entry/exit price, quantity, date, P&L
- **Options strategies** (single and multi-leg):
  - Short Put
  - Short Call
  - Covered Call
  - Cash-Secured Put
  - Bull Put Credit Spread
  - Bear Call Credit Spread
  - Iron Condor
  - Long Call / Long Put
  - Custom / other

### Key Fields per Trade Entry

- Ticker symbol
- Strategy type (enum)
- Open date / close date
- Expiration date (options)
- Strike(s) (options)
- Premium collected / paid
- Number of contracts / shares
- Realized P&L
- Notes / tags

## Main Window Layout

Three tabs in the main window (designed for easy future expansion):

1. **Trade Log**
2. **Analytics**
3. **Calendar**

### Theme Toggle

- Dark/light mode toggle button visible at all times (e.g., top-right of the title bar or nav header)
- Selected theme persisted to local settings (restored on next launch)
- Uses `ApplicationTheme` / `ElementTheme` via WinUI 3; setting stored in `Windows.Storage.ApplicationData.Current.LocalSettings`

---

## Trade Log Tab

A full-width `DataGrid` (WinUI Community Toolkit) showing all trades, **one row per trade**.

### Inline Editing

- Every cell is editable directly — clicking a cell activates edit mode for that field
- No separate "edit form"; changes are committed on focus loss or Enter key
- New rows added via an "Add Trade" button (appends a blank row to the grid)

### DataGrid Columns

| Column | Type | Notes |
|---|---|---|
| Ticker | Text | Auto-uppercase on input |
| Strategy | Dropdown | Bound to `StrategyType` enum (Short Put, Credit Spread, etc.) |
| Open Date | Date picker | |
| Close Date | Date picker | Nullable — empty when position is still open |
| Expiration Date | Date picker | Nullable — options only |
| Entry Price | Decimal | Per share / per contract premium |
| Closing Price | Decimal | Nullable — filled when position is closed |
| Contracts / Shares | Integer | |
| P/L ($) | Decimal | Realized profit/loss in dollars |
| G/L (%) | Decimal | Gain/loss as a percentage of capital at risk |
| Status | Dropdown | `Open` / `Closed` / `Assigned` |
| Notes | Text | Free-form, inline editable |

### Status Rules

- **Open**: position not yet closed
- **Closed**: position closed for a gain or loss
- **Assigned**: option was assigned (stock position taken or shares called away)

### Toolbar / Filters

A toolbar sits above the DataGrid with the following filters (all combinable, applied simultaneously):

- **Ticker** — free-text search (filters rows where ticker contains the typed text, case-insensitive)
- **Strategy** — multi-select dropdown (shows all `StrategyType` values; user can pick one or more)
- **Status** — multi-select dropdown (`Open`, `Closed`, `Assigned`; multiple selections valid)

Filtering is live (no submit button needed) and implemented in the ViewModel via a filtered `ObservableCollection` or `CollectionView`.

---

## Analytics Tab

### P/L and G/L Definitions

- **P/L** = Closing Price − Entry Price (per share or per contract, in dollars)
- **G/L** = P/L × 100 × Number of Contracts/Shares (total dollar gain/loss; the ×100 factor accounts for the standard options multiplier)

### Date Range Filter

Displayed at the top of the Analytics tab:

- All Time
- This Month
- This Year
- Custom (date range picker — start date and end date)

All charts and the summary stats row respect the selected date range.

### Summary Stats Row

A single row of KPI cards above the charts:

| Stat | Description |
|---|---|
| Total P/L | Sum of P/L (per-unit) across all filtered trades |
| Total G/L | Sum of G/L (total dollars) across all filtered trades |
| Total Trades | Count of trade rows matching the current filter |

### Charts (LiveCharts2)

Charts are arranged in a **responsive grid** that reflows based on available window width (e.g., 2-column on wide, 1-column on narrow).

| Chart | Type | Notes |
|---|---|---|
| P/L by Ticker | Bar chart | One bar per ticker; shows per-unit P/L |
| G/L by Ticker | Bar chart | One bar per ticker; shows total dollar G/L |
| P/L by Strategy | Bar chart | One bar per strategy type |
| Strategies Used | Pie chart | Slice per strategy; sized by trade count |

**Color coding (fixed, theme-independent):**
- Profitable trades / positive values → green
- Losing trades / negative values → red

This applies to bar chart fills and any row-level coloring — overrides both dark and light theme accent colors.

---

## Calendar Tab

### Monthly Calendar View

- Standard month grid (7 columns × up to 6 rows)
- Each day cell displays the **total G/L for that day** (sum of G/L across all trades with `CloseDate` on that day)
- Day cell color coding (fixed, theme-independent):
  - **Green** — net profitable day (total G/L > 0)
  - **Red** — net losing day (total G/L < 0)
  - **Grey** — no trades closed that day

### Navigation

- Previous / Next month buttons to move one month at a time
- Current month label displayed between the buttons

### Day Popover

- Clicking a day cell opens a popover (flyout) listing all trades closed on that day
- Each entry in the popover shows: Ticker, Strategy, G/L
- Popover is dismissible by clicking outside

---

## Features Summary

- Trade Log tab — inline-editable DataGrid with toolbar filters
- Analytics tab — responsive chart grid, summary stats, date range filter
- Calendar tab — monthly G/L heatmap with day-level trade popover
- Theme toggle (dark/light) with persistence
- Local SQLite database — no cloud dependency

---

## SQLite Schema

```sql
CREATE TABLE IF NOT EXISTS Trades (
    Id              INTEGER PRIMARY KEY AUTOINCREMENT,
    Ticker          TEXT    NOT NULL,
    StrategyType    INTEGER NOT NULL,   -- maps to StrategyType enum
    OpenDate        TEXT    NOT NULL,   -- ISO 8601: YYYY-MM-DD
    CloseDate       TEXT,               -- nullable; YYYY-MM-DD
    ExpirationDate  TEXT,               -- nullable; YYYY-MM-DD (options only)
    EntryPrice      TEXT    NOT NULL,   -- stored as TEXT to preserve decimal precision
    ClosingPrice    TEXT,               -- nullable
    Quantity        INTEGER NOT NULL,   -- number of contracts or shares
    PL              TEXT,               -- per-unit P/L (ClosingPrice - EntryPrice); TEXT for decimal
    GL              TEXT,               -- total dollar G/L (PL * 100 * Quantity); TEXT for decimal
    Status          INTEGER NOT NULL DEFAULT 0,  -- 0=Open, 1=Closed, 2=Assigned
    Notes           TEXT
);

CREATE TABLE IF NOT EXISTS Settings (
    Key     TEXT PRIMARY KEY,
    Value   TEXT NOT NULL
    -- Example keys: "AppTheme" ("Dark"/"Light")
);
```

### Schema Notes

- Money columns (`EntryPrice`, `ClosingPrice`, `PL`, `GL`) are stored as `TEXT` to avoid floating-point drift; parsed as `decimal` in C#.
- `StrategyType` and `Status` are stored as their `int` enum values.
- Dates are stored as ISO 8601 strings (`YYYY-MM-DD`) for easy sorting and range queries.
- `DatabaseInitializer` runs `CREATE TABLE IF NOT EXISTS` on startup — no separate migration files needed for the initial schema.
- Add a `SchemaVersion` row to `Settings` if schema migrations become necessary in the future.

## Development Guidelines

- Use `[ObservableProperty]` and `[RelayCommand]` source generators from CommunityToolkit.Mvvm
- Inject services via constructor (no service locator)
- Keep ViewModels testable — no direct WinUI type dependencies in ViewModels
- Use `async`/`await` throughout; never block the UI thread
- SQLite schema migrations handled in a dedicated `DatabaseInitializer` service
- All money values stored as `decimal` (never `double`)
