# Trading Journal

A personal Avalonia desktop app for logging and analyzing options and stock trades.

## Tech Stack

- **UI Framework**: Avalonia 12.0.1 with FluentTheme
- **Runtime**: .NET 9 (net9.0-windows, x64)
- **Language**: C#
- **MVVM**: CommunityToolkit.Mvvm — all logic lives in ViewModels; no logic in code-behind
- **Charts**: LiveChartsCore.SkiaSharpView.Avalonia
- **Data Grid**: Avalonia.Controls.DataGrid
- **Database**: Microsoft.Data.Sqlite (local storage)
- **UI files**: `.axaml` (Avalonia XAML); compiled bindings enabled globally via `AvaloniaUseCompiledBindingsByDefault=true`

## Architecture

MVVM strict mode — code-behind files contain only UI wiring (`InitializeComponent`, `DataContext` assignment). All state, commands, and business logic belong in ViewModels.

```
TradingJournal/
├── Models/           # Plain data classes (Trade, OptionLeg, Strategy, etc.)
├── ViewModels/       # CommunityToolkit.Mvvm ObservableObject subclasses
├── Views/            # AXAML pages and controls (minimal code-behind)
├── Services/         # Concrete implementations (SqliteTradeRepository, etc.)
├── Contracts/        # Service interfaces (ITradeRepository, ISettingsService, IDatabaseInitializer)
└── Assets/           # Icons, images
```

### Contracts

- `ITradeRepository` — `GetAllAsync`, `AddAsync`, `UpdateAsync`, `DeleteAsync`
- `ISettingsService` — `GetAsync(key)` / `SetAsync(key, value)` for the SQLite Settings table
- `IDatabaseInitializer` — `InitializeAsync()` called at app startup

ViewModels depend only on these interfaces; concrete implementations live in `Services/`.

### ViewLocator

`ViewLocator.cs` is already scaffolded. It resolves a ViewModel type to its corresponding View by replacing `ViewModel` with `View` in the fully-qualified type name. Follow this convention: every `FooViewModel` must have a matching `FooView` (UserControl or Window) in the `Views/` folder.

## Avalonia-Specific Patterns

### Compiled Bindings

`AvaloniaUseCompiledBindingsByDefault=true` is set in the project file. This means every `{Binding}` in AXAML is compiled by default. **`x:DataType` is mandatory on every View, UserControl, and DataTemplate** — omitting it causes silent binding failures at runtime.

```xml
<!-- Required on every root element and every DataTemplate -->
<UserControl x:DataType="vm:MyViewModel" ...>
<DataTemplate x:DataType="vm:CalendarDayViewModel">
```

Use `{ReflectionBinding}` only when a compiled binding is genuinely not possible.

### Theme

- Set via `Application.RequestedThemeVariant` at runtime (`"Light"`, `"Dark"`, `"Default"`)
- Persisted to the SQLite `Settings` table under key `"AppTheme"` using `ISettingsService`
- No `Windows.Storage` or any `Windows.*` namespace — keep ViewModels platform-agnostic

### DevTools

Avalonia DevTools are enabled in Debug builds (`WithDeveloperTools()` in `Program.cs`). Press F12 at runtime to inspect the visual/logical tree and live styles.

---

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
- Strike(s): stored as free-text to support both single-leg ("150") and multi-leg strategies ("150/160" for spreads, "145/150/160/165" for iron condors). No parsing or validation on this field — entered exactly as the user types it.

---

## Main Window Layout

Three tabs in the main window (designed for easy future expansion):

1. **Trade Log**
2. **Analytics**
3. **Calendar**

### Theme Toggle

- Dark/light mode toggle button visible at all times (top-right of the nav header)
- Calls `Application.Current.RequestedThemeVariant = ThemeVariant.Light / Dark`
- Selected theme read from and written to the SQLite `Settings` table on startup and change

---

## Trade Log Tab

A full-width `DataGrid` (`Avalonia.Controls.DataGrid`) showing all trades, **one row per trade**.

### Inline Editing

- Every cell is editable directly — clicking a cell activates edit mode for that field
- No separate "edit form"; changes are committed on focus loss or Enter key
- New rows added via an "Add Trade" button (appends a blank row to the grid)

### DataGrid Columns

| Column | Type | Notes |
|---|---|---|
| Ticker | Text | Auto-uppercase on input |
| Strike Price| Text | Nullable; single strike or slash-separated for multi-leg e.g. "150/160", "145/150/160/165" |
| Strategy | Dropdown | Bound to `StrategyType` enum |
| Open Date | Date picker | |
| Close Date | Date picker | Nullable — empty when position is still open |
| Expiration Date | Date picker | Nullable — options only |
| Entry Price | Decimal | Per share / per contract premium |
| Closing Price | Decimal | Nullable — filled when position is closed |
| Contracts / Shares | Integer | |
| P/L ($) | Decimal | Realized profit/loss per unit |
| G/L ($) | Decimal | Total dollar gain/loss |
| Status | Dropdown | `Open` / `Closed` / `Assigned` |
| Notes | Text | Free-form, inline editable |

### Status Rules

- **Open**: position not yet closed
- **Closed**: position closed for a gain or loss
- **Assigned**: option was assigned (stock position taken or shares called away)

### Deleting Trades

- Right-click a row to open a context menu with a "Delete Trade" option
- A confirmation dialog is shown before deletion to prevent accidents
- Deletion removes the row from both the `ObservableCollection` and the database

### Toolbar / Filters

A toolbar sits above the DataGrid with the following filters (all combinable, applied simultaneously):

- **Ticker** — free-text search (case-insensitive contains)
- **Strategy** — multi-select dropdown (all `StrategyType` values)
- **Status** — multi-select dropdown (`Open`, `Closed`, `Assigned`)

Filtering is live (no submit button) and implemented in the ViewModel via a filtered `ObservableCollection` or `CollectionView`.

---

## Analytics Tab

### P/L and G/L Definitions

- **P/L** = Closing Price − Entry Price (per share or per contract, in dollars)
- **G/L** = P/L × 100 × Number of Contracts/Shares (total dollar gain/loss; ×100 is the standard options multiplier)

### Date Range Filter

Displayed at the top of the Analytics tab:

- All Time
- This Month
- This Year
- Custom (date range picker — start date and end date)

All charts and the summary stats row respect the selected date range.

### Summary Stats Row

| Stat | Description |
|---|---|
| Total P/L | Sum of per-unit P/L across all filtered trades |
| Total G/L | Sum of total dollar G/L across all filtered trades |
| Total Trades | Count of rows matching the current filter |

### Charts (LiveChartsCore.SkiaSharpView.Avalonia)

Charts are arranged in a responsive grid that reflows based on available window width (2-column wide, 1-column narrow).

| Chart | Type | Notes |
|---|---|---|
| Equity Curve | Line chart | Cumulative G/L over time, one point per closed trade |
| P/L by Ticker | Bar chart | One bar per ticker |
| G/L by Ticker | Bar chart | One bar per ticker |
| P/L by Strategy | Bar chart | One bar per strategy type |
| Strategies Used | Pie chart | Slice per strategy, sized by trade count |

**Color coding (fixed, theme-independent):**
- Positive values → green
- Negative values → red

---

## Calendar Tab

### Monthly Calendar View

- Standard month grid built with `ItemsControl` + `UniformGrid` (Columns=7) as the items panel
- Always 42 items (6 weeks × 7 days) — padding days from the previous/next month are shown greyed out
- Each day cell displays the **total G/L for that day** (sum across all trades with `CloseDate` on that day)

**ViewModel structure:**

```
CalendarViewModel
├── CurrentYear, CurrentMonth  (int)
├── CurrentMonthLabel          ("April 2026")
├── PreviousMonthCommand / NextMonthCommand
└── Days : ObservableCollection<CalendarDayViewModel>  (always 42 items)

CalendarDayViewModel
├── Date            (DateOnly)
├── DayNumber       (int)
├── IsCurrentMonth  (bool — false for padding days)
├── TotalGL         (decimal? — null means no trades that day)
├── Trades          (List<TradeSummary> — for the day popover)
└── CellColor       (computed IBrush: green / red / grey)
```

When the month changes, `Days` is rebuilt: find the weekday of the 1st, backfill with days from the prior month, fill the current month, pad to 42 with next-month days.

**Day cell color coding (fixed, theme-independent):**
- Green — net profitable day (G/L > 0)
- Red — net losing day (G/L < 0)
- Grey — no trades closed that day, or padding day

### Navigation

- Previous / Next month `Button`s
- Current month label between them

### Locale

All calendar text must be in English regardless of the system locale (Windows may be set to a non-English language). Use `CultureInfo.InvariantCulture` (or `new CultureInfo("en-US")`) for all date formatting in `CalendarViewModel`:
- Month label: `date.ToString("MMMM yyyy", CultureInfo.InvariantCulture)`
- Day-of-week headers: hardcoded string array `["Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"]`

### Day Popover

Clicking a day cell opens a Flyout anchored to that cell.
The Flyout contains a scrollable list of all trades closed that day.
Each entry shows: Ticker, Strategy, G/L.
The Flyout closes when clicking outside it.
No hover behavior needed.

### Flyouts
Use `FlyoutBase.AttachedFlyout` on day cells in the Calendar tab.
Open programmatically via `FlyoutBase.ShowAttachedFlyout(control)` 
in the code-behind on PointerPressed — this is one of the rare 
acceptable uses of code-behind in this project.
---

## SQLite Schema

```sql
CREATE TABLE IF NOT EXISTS Trades (
    Id              INTEGER PRIMARY KEY AUTOINCREMENT,
    Ticker          TEXT    NOT NULL,
    StrategyType    INTEGER NOT NULL,   -- maps to StrategyType enum
    Strike          TEXT,               -- nullable; single value "150" or multi-leg "150/160"
    OpenDate        TEXT    NOT NULL,   -- ISO 8601: YYYY-MM-DD
    CloseDate       TEXT,               -- nullable; YYYY-MM-DD
    ExpirationDate  TEXT,               -- nullable; YYYY-MM-DD (options only)
    EntryPrice      TEXT    NOT NULL,   -- stored as TEXT to preserve decimal precision
    ClosingPrice    TEXT,               -- nullable
    Quantity        INTEGER NOT NULL,   -- number of contracts or shares
    PL              TEXT,               -- per-unit P/L; TEXT for decimal precision
    GL              TEXT,               -- total dollar G/L; TEXT for decimal precision
    Status          INTEGER NOT NULL DEFAULT 0,  -- 0=Open, 1=Closed, 2=Assigned
    Notes           TEXT
);

CREATE TABLE IF NOT EXISTS Settings (
    Key     TEXT PRIMARY KEY,
    Value   TEXT NOT NULL
    -- Used keys: "AppTheme" ("Dark"/"Light"), "SchemaVersion"
);
```

### Schema Notes

- Money columns stored as `TEXT` to avoid floating-point drift; parsed as `decimal` in C#.
- `StrategyType` and `Status` stored as their `int` enum values.
- Dates stored as ISO 8601 strings (`YYYY-MM-DD`) for easy sorting and range queries.
- `DatabaseInitializer` runs `CREATE TABLE IF NOT EXISTS` on startup — no separate migration files for the initial schema.
- Add a `SchemaVersion` row to `Settings` when schema migrations become necessary.

---

## Development Guidelines

- Every public type (class, interface, enum, record), member (property, method, constructor), and enum value **must** have an XML `<summary>` doc comment. Use `<param>`, `<returns>`, and `<remarks>` where they add genuine clarity.
- Use `[ObservableProperty]` and `[RelayCommand]` source generators from CommunityToolkit.Mvvm
- Inject services via constructor (no service locator)
- ViewModels must have zero dependencies on Avalonia types — keeps them testable and platform-agnostic
- `x:DataType` is required on every View, UserControl, and DataTemplate (compiled bindings)
- Use `async`/`await` throughout; never block the UI thread
- All money values stored and computed as `decimal` (never `double` or `float`)
- No `Windows.*` namespace usage in ViewModels or Services
- Follow the ViewLocator convention: `FooViewModel` → `FooView` (UserControl in `Views/`)

## Data Storage

The SQLite database file is stored at `%APPDATA%\TradingJournal\trades.db`.
The directory is created on first launch if it does not exist.
This location is writable without admin rights, survives app updates, and is safe from accidental git commits.

---

## Distribution

**Current target:** Single-file `.exe` (`PublishSingleFile` in .NET 9) — simplest for personal use.

## Database File — Debug vs Release

To protect real trade data during development, the database filename differs by build configuration:

- **Debug** → `%APPDATA%\TradingJournal\trades-dev.db`
- **Release** → `%APPDATA%\TradingJournal\trades.db`

Implemented in `DatabaseInitializer` using a compile-time `#if DEBUG` / `#else` block.
Running in Visual Studio (Debug) will never touch the production database.

**Future (final polish step): MSIX installer**
- Add a Windows Application Packaging Project (`.wapproj`) to the solution in Visual Studio
- Configure `Package.appxmanifest` (name, publisher, version, icons, capabilities)
- Sign with a self-signed certificate (personal use) or a commercial certificate (wider distribution)
- Output is a single `.msix` file — double-click installs, registers a Start Menu entry, supports clean uninstall
- No application code changes required — MSIX is purely a packaging layer
- `%APPDATA%` is fully accessible from MSIX packaged apps, so the database path needs no adjustment

---

## Build Order (for Claude Code sessions)
1. Models — StrategyType enum, TradeStatus enum, Trade class
2. Data layer — Contracts interfaces, DatabaseInitializer,
   SqliteTradeRepository, SqliteSettingsService, schema created on startup
3. Scaffold — MainWindow, TabControl with 3 empty tabs, theme toggle
   wired to ISettingsService, ViewLocator convention verified
4. Trade Log — DataGrid with ObservableCollection<Trade>, all columns,
   dummy data, inline editing, Add Trade button
5. Trade Log — Filters: ticker text search, strategy multi-select,
   status multi-select, all live-filtering in ViewModel
6. Analytics — Stats row: Total P/L, Total G/L, Total Trades,
   date range filter wired up
7. Analytics — Charts: equity curve first, then P/L by ticker,
   G/L by ticker, P/L by strategy, strategies pie
8. Calendar — monthly grid 42 cells, G/L per day, color coding,
   month navigation, Flyout popover showing trades
9. Polish — input validation, empty states, error handling