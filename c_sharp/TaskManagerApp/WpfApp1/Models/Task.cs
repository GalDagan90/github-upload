using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TaskManagerApp.Models
{
    public enum MyTaskStatus
    {
        NotStarted,
        InProgress,
        Completed,
    }

    public enum MyTaskPriority
    {
        Low,
        Medium,
        High,
    }
    
    public class Task
    {
        public Guid Id { get; } = Guid.NewGuid();
        public string Title { get; set; }
        public string Description { get; set; }
        public MyTaskStatus Status { get; set; } = MyTaskStatus.NotStarted;
        public MyTaskPriority Priority { get; set; } = MyTaskPriority.Low;
        public DateTime CreatedAt { get; } = DateTime.Now;
        public DateTime DueDate { get; set; }
        
        public Task(string title,
                    string description, 
                    DateTime dueDate,
                    MyTaskStatus status = MyTaskStatus.NotStarted,
                    MyTaskPriority priority = MyTaskPriority.Low)
        {
            Title = title;
            Description = description;
            DueDate = dueDate;
            Status = status;
            Priority = priority;
        }

        public Task()
        {
            Title = "New Task";
            Description = "No description provided.";
            DueDate = DateTime.Now.AddDays(7); // Default due date is one week from now
        }
        public override string ToString()
        {
            return $"{Title} - {Status} - {Priority} - Due: {DueDate.ToShortDateString()}";
        }
    }
}
